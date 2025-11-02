#include "ui_diver.h"
#include "weather.h"
#include "lvgl.h"
#include "ui.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "wifi_manager.h"
#include "dht11.h"


#define WEATHER_DEFAULT_CITY "nanchang"
#define IMG_ZOOM_SIZE 160
#define WEATHER_GET_PERIOD 60*1000

const char* icon[] = { "sunny","cloud","overcast","rain","snow","windy","unknow" };

static lv_timer_t* weather_timer = NULL;
static lv_timer_t* other_card_timer = NULL;
static char location[30] = WEATHER_DEFAULT_CITY;

static lv_obj_t* weather_icon_arr[3];

static TaskHandle_t weather_task_handle;

void set_weather_unknow(lv_obj_t* parent)
{
    if (parent == NULL) return;

    uint32_t count = lv_obj_get_child_cnt(parent);
    if (count == 0) return;  //没有子对象就退出

    for (uint32_t i = 0; i < count; i++)
    {
        lv_obj_t* child = lv_obj_get_child(parent, i);

        if (lv_obj_check_type(child, &lv_label_class))
        {
            lv_label_set_text(child, "--");
        }
        else if (lv_obj_check_type(child, &lv_image_class))
        {
            uint16_t zoom = lv_img_get_zoom(child);
            lv_img_set_src(child, "/img/icon/unknow.png");
            lv_img_set_zoom(child, zoom);
        }

        // 确保递归不会进入已经递归过的对象或自己
        if (child != parent)
            set_weather_unknow(child);
    }
}

static int8_t get_weather_forecast_code(weather_forecast_t* weather_forecast, uint8_t i)
{
    return weather_forecast->day[i].code;
}

static uint8_t get_weather_icon_by_code(uint8_t code)
{
    if (code == 0 && code == 1) return 0;     //sunny

    else if (code >= 4 && code <= 8) return 1;  //cloud

    else if (code == 9) return 2;   //overcast

    else if (code >= 13 && code <= 18) return 3;    //rainy

    else if (code == 20 || (code >= 22 && code <= 25)) return 4;  //snow

    else if (code == 32 || code == 33) return 5; //windy

    else return 6;   //unknow
}

void flush_weather_ui_data()
{
    /* 刷新温度 */

    if (weather.temp == -1 || weather.code == -1)
    {
        set_weather_unknow(ui_weather_handle.weather_card_root);
        lv_label_set_text(ui_weather_handle.today_label, "今天");
        lv_label_set_text(ui_weather_handle.tomorrow_label, "明天");
        lv_label_set_text(ui_weather_handle.after_label, "后天");
        return;
    }
    /* ------------ 设置文字 ------------ */
    lv_label_set_text(ui_weather_handle.today_label, "今天");
    lv_label_set_text(ui_weather_handle.tomorrow_label, "明天");
    lv_label_set_text(ui_weather_handle.after_label, "后天");

    /* ------------ 设置温度 ------------ */
    lv_label_set_text_fmt(ui_weather_handle.today_temp, "%d°C", weather.temp);    //设置上半天气温度
    //设置下半今日最高低
    lv_label_set_text_fmt(ui_weather_handle.today_temp2, "%d°/%d°",
        weather_forecast.day[0].high,
        weather_forecast.day[0].low);
    //设置下半明日最高低
    lv_label_set_text_fmt(ui_weather_handle.tomorrow_temp, "%d°/%d°",
        weather_forecast.day[1].high,
        weather_forecast.day[1].low);
    //设置下半后天最高低
    lv_label_set_text_fmt(ui_weather_handle.after_temp, "%d°/%d°",
        weather_forecast.day[2].high,
        weather_forecast.day[2].low);

    /* ------------ 设置图标（第一次设置，随后有变化再改） ------------ */
    //设置上半图标
    char icon_path[30];
    snprintf(icon_path, sizeof(icon_path), "/img/icon/%s.png", icon[get_weather_icon_by_code(weather.code)]);
    printf("%p", ui_weather_handle.today_icon);
    lv_img_set_src(ui_weather_handle.today_icon, icon_path);
    // printf("")
    //设置下半图标
    for (uint8_t i = 0;i < 3;++i)
        printf("%p ", icon[i]);
    for (uint8_t i = 0;i < 3;++i)
    {
        memset(icon_path, 0, sizeof(icon_path));
        int8_t code = get_weather_forecast_code(&weather_forecast, i);
        snprintf(icon_path, sizeof(icon_path), "/img/icon/%s.png", icon[get_weather_icon_by_code(code)]);
        lv_img_set_src(weather_icon_arr[i], icon_path);
        lv_img_set_zoom(weather_icon_arr[i], IMG_ZOOM_SIZE);
    }

}

static void weather_timer_cb(lv_timer_t* timer)
{
    /* ------------ 刷新数据到weather ui ------------ */
    if (root_screen != lv_screen_active())
        return;
    /* 刷新温度 */
    flush_weather_ui_data();

}

void weather_ui_bind_icons(void)
{
    weather_icon_arr[0] = ui_weather_handle.today_icon2;
    weather_icon_arr[1] = ui_weather_handle.tomorrow_icon;
    weather_icon_arr[2] = ui_weather_handle.after_icon;
}

static void weather_get_task(void* param)
{
    while (1)
    {
        weather_get(location, &weather, &weather_forecast);
        vTaskDelay(pdMS_TO_TICKS(WEATHER_GET_PERIOD));
    }

}

static void weather_trigger_first_request(void* param)
{
    EventBits_t bits;
    bits = xEventGroupWaitBits(wifi_manager_get_event_group(),
        WIFI_CONNECTED_BIT,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY);
    if (bits & WIFI_CONNECTED_BIT)
    {
        /* 马上更新一次数据 */
        weather_get(location, &weather, &weather_forecast);
        if (root_screen == lv_screen_active())
        {
            lv_async_call((lv_async_cb_t)flush_weather_ui_data, NULL);
            // flush_weather_ui_data();
        }
        /* 创建timer和weather get任务 */
        xTaskCreatePinnedToCore(weather_get_task,
            "weather_get_task",
            4096,
            WEATHER_DEFAULT_CITY,
            5,
            &weather_task_handle,
            APP_CPU_NUM);
        weather_timer = lv_timer_create(weather_timer_cb, WEATHER_GET_PERIOD, location);
    }
    vTaskDelete(NULL);
}


void flush_other_card_data(void)
{
    int room_temp = dth11_get_room_temp();
    int humidity = dht11_get_humidity();
    if ((room_temp == -1) || (humidity == -1))
        lv_label_set_text_fmt(ui_weather_handle.scroll_label, "室温:--°C 湿度:--%% 今天团子也给你加油ヾ(*´v ˋ*)ﾉ");
    else
        lv_label_set_text_fmt(ui_weather_handle.scroll_label, "室温:%d.%d°C 湿度:%d%% 今天团子也给你加油ヾ(*´v ˋ*)ﾉ", room_temp / 10, room_temp % 10, humidity);
}

static void other_card_timer_cb(lv_timer_t* timer)
{
    if (root_screen != lv_screen_active())
        return;
    /* 刷新室温、湿度 */
    flush_other_card_data();
}

/* 初始化天气驱动 */
void weather_dirver_init()
{

    other_card_timer = lv_timer_create(other_card_timer_cb, 10 * 1000, NULL);
    xTaskCreatePinnedToCore(weather_trigger_first_request,
        "weather_trigger_first_request",
        4096,
        WEATHER_DEFAULT_CITY,
        6,
        &weather_task_handle,
        APP_CPU_NUM);
}

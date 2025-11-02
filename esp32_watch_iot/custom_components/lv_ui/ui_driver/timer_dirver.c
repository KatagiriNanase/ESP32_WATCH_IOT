#include "ui_diver.h"
#include "time_manager.h"
#include <time.h>
#include "lvgl.h"
#include "ui.h"

struct tm* now_tm=NULL;

static uint8_t home_count = 0;
static void lvgl_timer_cb(lv_timer_t* timer)
{
    /* 获取当前时间 */
    // if(time_manager_is_time_synced())
    time_t now;
    time(&now);
    now_tm = localtime(&now);

    if (ui_clock_handle.root == lv_screen_active())
    {
        /* 刷新当前时间 */
        lv_label_set_text_fmt(ui_clock_handle.hour, "%02d", now_tm->tm_hour);
        lv_label_set_text_fmt(ui_clock_handle.min, "%02d", now_tm->tm_min);
        lv_label_set_text_fmt(ui_clock_handle.sec, "%02d", now_tm->tm_sec);
    }
    else if (root_screen == lv_screen_active())
    {
        if (home_count++ == 0)
            return;
        /* 刷新当前时间 */
        lv_label_set_text_fmt(ui_home_handle.hour, "%02d", now_tm->tm_hour);
        lv_label_set_text_fmt(ui_home_handle.min, "%02d", now_tm->tm_min);
        lv_label_set_text_fmt(ui_home_handle.month, "%02d", now_tm->tm_mon+1);
        lv_label_set_text_fmt(ui_home_handle.day, "%02d", now_tm->tm_mday);
    }

}

void lvgl_time_driver(void)
{
    /* 创建定时器 */
    lv_timer_create(lvgl_timer_cb, 1000, NULL);
}
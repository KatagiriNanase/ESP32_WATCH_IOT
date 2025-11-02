#include <stdio.h>
#include <string.h>
#include "time_manager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "weather.h"
#include "lv_port.h"
#include "spiffs_manager.h"
#include "ui.h"
#include "ui_diver.h"
#include "dht11.h"
#include "mqtt.h"

#define TAG "main"
#define DHT11_PIN 32

void app_main(void)
{
    // ESP_LOGI("main", "Display LVGL Meter Widget");
    // spiffs_manager_init();
    wifi_manager_init_sta();
    time_manager_init();
    DHT11_Init(DHT11_PIN);
    mqtt_init();
    // lv_port_init();
    // lvgl_time_driver();
    // weather_http_init();
    // weather_dirver_init();
    // set_up_src_clock();
    
    /* lv_obj_t* img = lv_img_create(lv_screen_active());
    lv_img_set_src(img, "/img/icon/wallpaper.png");
    lv_obj_set_pos(img, 0, 0); */

    // while (1)
    // {
        
    //     vTaskDelay(pdMS_TO_TICKS(1000));
    // }


}

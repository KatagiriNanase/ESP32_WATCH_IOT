#include <stdio.h>
// #include "time_manager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "weather.h"
void app_main(void)
{

    // wifi_manager_init_sta();
    // time_manager_init();
    // time_t now;

    while (1)
    {
        // DHT11_StartGet(&tempX10, &humidity);
        // ESP_LOGI("main", "temp:%i.%i humidity:%d", tempX10 / 10, tempX10 % 10, humidity);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }


}

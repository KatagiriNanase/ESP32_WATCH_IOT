#include "time_manager.h"
#include "esp_netif_sntp.h"
#include "esp_sntp.h"
#include "lwip/ip_addr.h"

static const char* TAG = "time_manager";
#define CONFIG_SNTP_TIME_SERVER "pool.ntp.org"
#define RETRY_COUNT 3

/* 检测周期 */
#define DETECT_INTERVAL 5000   //3000ms

static EventGroupHandle_t s_sntp_event_group;
#define SNTP_SUCCESS_BIT BIT0
#define SNTP_FAIL_BIT BIT1

static void print_servers(void)
{
    ESP_LOGI(TAG, "List of configured NTP servers:");

    for (uint8_t i = 0; i < SNTP_MAX_SERVERS; ++i)
    {
        if (esp_sntp_getservername(i))
        {
            ESP_LOGI(TAG, "server %d: %s", i, esp_sntp_getservername(i));
        }

    }
}

static void time_sync_notification_cb(struct timeval* tv)
{
    ESP_LOGI(TAG, "SNTP time synchronized!");
    xEventGroupSetBits(s_sntp_event_group, SNTP_SUCCESS_BIT);
}

static void time_manager_sync_task(void* param)
{
    static uint16_t retry_num = 0;
    while (!time_manager_is_time_synced())
    {
        /* 开始周期性同步 */
        ESP_LOGI(TAG, "Retry sntp:%d", retry_num++);
        time_manager_sntp_start();
        esp_netif_sntp_sync_wait(pdMS_TO_TICKS(DETECT_INTERVAL));
    }
    /* 连上后就删除任务自身 */
    if (esp_sntp_get_sync_status() == SNTP_SYNC_STATUS_COMPLETED && time_manager_is_time_synced())
        vTaskDelete(NULL);
}

esp_err_t time_manager_sntp_start(void)
{
    esp_err_t ret;
    if (wifi_manager_is_connected())
    {
        ret = esp_netif_sntp_start();
        ESP_LOGI(TAG, "stnp start!");
        return ret;
    }
    else
    {
        xEventGroupSetBits(s_sntp_event_group, SNTP_FAIL_BIT);
        ESP_LOGI(TAG, "wifi not connect!");
        return ESP_FAIL;
    }
}

void time_manager_init(void)
{
    /* 初始化event句柄 */
    s_sntp_event_group = xEventGroupCreate();
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG(CONFIG_SNTP_TIME_SERVER);
    config.start = false; // 延迟启动（等待 Wi-Fi 连接）
    config.sync_cb = time_sync_notification_cb;
    esp_netif_sntp_init(&config);
    print_servers();

    /* 进行一次时间同步 */
    time_manager_sntp_start();

    /* 调整时区 */
    setenv("TZ", "CST-8", 1);
    tzset();

    /* 创建周期时间同步任务 */
    xTaskCreatePinnedToCore(time_manager_sync_task, "time_manager_sync_task", 2048, NULL, 5, NULL, APP_CPU_NUM);
}

bool time_manager_is_time_synced(void)
{
    EventBits_t bits = xEventGroupGetBits(s_sntp_event_group);
    return bits & SNTP_SUCCESS_BIT;
}

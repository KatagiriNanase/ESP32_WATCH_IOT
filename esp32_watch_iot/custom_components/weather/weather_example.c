#include "weather.h"
#include "wifi_manager.h"
#include <string.h>
#include <cJSON.h>

#define MAX_HTTP_OUTPUT_BUFFER 2048
#define WEATHER_GET_URL "https://api.seniverse.com/v3/weather/now.json?key=your_api_key&location=beijing&language=zh-Hans&unit=c"
#define WEATHER_API_KEY "YOUR_API_KEY"
static const char* TAG = "weather";

static char http_data_buffer[MAX_HTTP_OUTPUT_BUFFER + 1] = { 0 };
static int http_total_len = 0;
static esp_http_client_handle_t weather_http_client;

#define HTTP_ON_DATA_BIT BIT0
static EventGroupHandle_t http_event_group;

weather_t weather = { -1 ,-1 };
weather_forecast_t weather_forecast = { {{-1,-1,-1},{-1,-1,-1},{-1,-1,-1} } };

static esp_err_t _weather_http_event_handler(esp_http_client_event_t* evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        // Clean the buffer in case of a new request
        ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        if (evt->data && evt->data_len > 0)
        {
            // 注意防止溢出
            if (http_total_len + evt->data_len < sizeof(http_data_buffer))
            {
                memcpy(http_data_buffer + http_total_len, evt->data, evt->data_len);
                http_total_len += evt->data_len;
            }
            else
            {
                ESP_LOGE(TAG, "HTTP buffer overflow!");
            }
        }
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH, total len=%d", http_total_len);
        http_data_buffer[http_total_len] = '\0';  // 添加字符串结束符
        http_total_len = 0;
        xEventGroupSetBits(http_event_group, HTTP_ON_DATA_BIT);
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
        break;

    default:
        break;
    }
    return ESP_OK;
}

esp_err_t weather_http_init(void)
{
    /* 初始化weather事件 */
    http_event_group = xEventGroupCreate();
    return ESP_OK;
}

esp_err_t weather_http_perform(const char* url, esp_http_client_method_t method)
{
    esp_err_t ret;
    esp_http_client_set_url(weather_http_client, url);
    esp_http_client_set_method(weather_http_client, method);
    ret = esp_http_client_perform(weather_http_client);
    return ret;
}

void weather_url_concat(const char* location, char* url)
{
    if (!url)
    {
        ESP_LOGE(TAG, "weather_url_concat:char*url is NULL");
        return;
    }
    char local_url[256];
    memset(local_url, 0, sizeof(local_url));

    snprintf(local_url, sizeof(local_url), "https://api.seniverse.com/v3/weather/now.json?key=%s&location=%s&language=zh-Hans&unit=c", WEATHER_API_KEY, location);

    memcpy(url, local_url, strlen(local_url));
}

void weather_forecast_url_concat(const char* location, char* url)
{
    if (!url)
    {
        ESP_LOGE(TAG, "weather_url_concat:char*url is NULL");
        return;
    }
    char local_url[256];
    memset(local_url, 0, sizeof(local_url));

    snprintf(local_url, sizeof(local_url), "https://api.seniverse.com/v3/weather/daily.json?key=%s&location=%s&language=zh-Hans&unit=c&start=0&days=3", WEATHER_API_KEY, location);

    memcpy(url, local_url, strlen(local_url));
}

/**
 * @brief 解析天气实况json
 *
 * @param weather
 */
static void weather_parse(weather_t* weather)
{
    {
        /* "results": [
        {
            "location": {
                "id": "C23NB62W20TF",
                    "name" : "南昌",
                    "country" : "CN",
                    "path" : "南昌,南昌,江西,中国",
                    "timezone" : "Asia/Shanghai",
                    "timezone_offset" : "-07:00"
            },
                "now": {
                "text": "多云", //天气现象文字
                    "code" : "4", //天气现象代码
                    "temperature" : "14", //温度，单位为c摄氏度或f华氏度
            },
                "last_update": "2025-10-28T09:17:36+08:00" //数据更新时间（该城市的本地时间）
        }
        ] */
    }
    cJSON* root = cJSON_Parse(http_data_buffer);
    cJSON* results = cJSON_GetObjectItem(root, "results");
    cJSON* arr1 = cJSON_GetArrayItem(results, 0);
    cJSON* now = cJSON_GetObjectItem(arr1, "now");
    cJSON* code = cJSON_GetObjectItem(now, "code");
    cJSON* temp = cJSON_GetObjectItem(now, "temperature");

    if (!root || !results || !arr1 || !now || !code || !temp)
    {
        ESP_LOGE(TAG, "JSON parse error");
        if (root) cJSON_Delete(root);
        return;
    }

    /* 将text转换为整型 */
    weather->code = atoi(cJSON_GetStringValue(code));
    weather->temp = atoi(cJSON_GetStringValue(temp));

    /* 释放内存 */
    cJSON_Delete(root);

}

static void weather_forecast_parse(weather_forecast_t* weather)
{
    if (weather == NULL)
    {
        return;
    }

    // 1. 解析JSON字符串为cJSON对象
    cJSON* root = cJSON_Parse(http_data_buffer);
    if (root == NULL)
    {
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
            ESP_LOGE(TAG, "forecast json parse fail!");
        return;
    }

    // 2. 逐层获取JSON节点
    cJSON* results = cJSON_GetObjectItem(root, "results");
    if (cJSON_IsArray(results) && cJSON_GetArraySize(results) > 0)
    {
        // 获取第一个results对象（通常只有一个城市结果）
        cJSON* result_obj = cJSON_GetArrayItem(results, 0);
        if (result_obj != NULL)
        {
            // 获取daily数组（存储每日天气数据）
            cJSON* daily = cJSON_GetObjectItem(result_obj, "daily");
            if (cJSON_IsArray(daily))
            {
                int daily_count = cJSON_GetArraySize(daily);
                // 最多解析3天数据（今天+未来两天）
                int parse_days = (daily_count < 3) ? daily_count : 3;

                for (int i = 0; i < parse_days; i++)
                {
                    cJSON* day_item = cJSON_GetArrayItem(daily, i);
                    if (day_item == NULL)
                    {
                        continue;
                    }

                    // 3. 提取各字段值（转为整数）
                    cJSON* code_day = cJSON_GetObjectItem(day_item, "code_day");
                    cJSON* high = cJSON_GetObjectItem(day_item, "high");
                    cJSON* low = cJSON_GetObjectItem(day_item, "low");

                    // 赋值到结构体（若字段不存在则默认-1）
                    weather->day[i].code = cJSON_IsString(code_day) ? atoi(code_day->valuestring) : -1;
                    weather->day[i].high = cJSON_IsString(high) ? atoi(high->valuestring) : -1;
                    weather->day[i].low = cJSON_IsString(low) ? atoi(low->valuestring) : -1;
                }
            }
        }
    }

    // 4. 释放cJSON对象（避免内存泄漏）
    cJSON_Delete(root);

}


esp_err_t weather_get(const char* location, weather_t* weather, weather_forecast_t* weather_forecast)
{
    assert(location);
    assert(weather);
    assert(weather_forecast);

    /* ---------- 获取当前天气、温度 ---------- */
    esp_http_client_config_t config = {
        .event_handler = _weather_http_event_handler,
        .url = "https://api.seniverse.com",
        .disable_auto_redirect = true,
        .timeout_ms = 10000,
    };
    weather_http_client = esp_http_client_init(&config);
    if (!weather_http_client)
    {
        ESP_LOGI("weather", "http init fail!");
        return ESP_FAIL;
    }

    /* 判断wifi是否连接成功 */
    if (!wifi_manager_is_connected())
    {
        ESP_LOGI(TAG, "weather cannot get due to false of wifi");
        wifi_manager_connect();
        esp_http_client_cleanup(weather_http_client);
        return ESP_FAIL;
    }

    char local_url[256] = { 0 };
    weather_url_concat(location, local_url);
    /* 发送请求 */
    ESP_ERROR_CHECK(weather_http_perform(local_url, HTTP_METHOD_GET));
    /* 阻塞等待 */
    EventBits_t bits = xEventGroupWaitBits(http_event_group, HTTP_ON_DATA_BIT, pdTRUE, pdFALSE, pdMS_TO_TICKS(5000));
    if ((bits & HTTP_ON_DATA_BIT) == 0)
    {
        ESP_LOGI(TAG, "cannot get http data!");
        esp_http_client_cleanup(weather_http_client);
        return ESP_FAIL;
    }
    else if (bits & HTTP_ON_DATA_BIT)
    {
        weather_parse(weather);
        ESP_LOGI(TAG, "get weather data!");
    }

    memset(http_data_buffer, 0, sizeof(http_data_buffer));

    /* ---------- 获取未来三天天气 ---------- */
    /* 清0local_url */
    memset(local_url, 0, sizeof(local_url));
    weather_forecast_url_concat(location, local_url);
    /* 发送请求 */
    ESP_ERROR_CHECK(weather_http_perform(local_url, HTTP_METHOD_GET));
    /* 阻塞等待 */
    bits = xEventGroupWaitBits(http_event_group, HTTP_ON_DATA_BIT, pdTRUE, pdFALSE, pdMS_TO_TICKS(5000));
    if ((bits & HTTP_ON_DATA_BIT) == 0)
    {
        ESP_LOGI(TAG, "cannot get http data!,Line:%d", __LINE__);
        esp_http_client_cleanup(weather_http_client);
        return ESP_FAIL;
    }
    else if (bits & HTTP_ON_DATA_BIT)
    {
        ESP_LOGI("forecast", "%s", http_data_buffer);
        weather_forecast_parse(weather_forecast);
        ESP_LOGI(TAG, "get weather data!");
    }

    esp_http_client_cleanup(weather_http_client);
    return ESP_OK;

}
#include "weather.h"
#include "wifi_manager.h"
#include <string.h>


#define MAX_HTTP_OUTPUT_BUFFER 2048
#define WEATHER_GET_URL "https://api.seniverse.com/v3/weather/now.json?key=your_api_key&location=beijing&language=zh-Hans&unit=c"
#define WEATHER_API_KEY "SRZaSphzHFeRhW87j"
static const char* TAG = "weather";

static char http_data_buffer[MAX_HTTP_OUTPUT_BUFFER + 1] = { 0 };
static esp_http_client_handle_t weather_http_client;
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
        if (evt->data)
        {
            // we are just starting to copy the output data into the use
            memset(http_data_buffer, 0, MAX_HTTP_OUTPUT_BUFFER);

            if (MAX_HTTP_OUTPUT_BUFFER > evt->data_len)
                memcpy(http_data_buffer, evt->data, evt->data_len);
            else
                memcpy(http_data_buffer, evt->data, MAX_HTTP_OUTPUT_BUFFER);
            ESP_LOGI("HTTP DATA", "%s", http_data_buffer);
        }
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
    esp_http_client_config_t config = {
        .event_handler = _weather_http_event_handler,
        .disable_auto_redirect = true,
    };
    ESP_LOGI(TAG, "HTTP request with url =>");
    weather_http_client = esp_http_client_init(&config);
    if (weather_http_client)
        return ESP_OK;
    else
    {
        ESP_LOGI("weather", "http init fail!");
        return ESP_FAIL;
    }
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

esp_err_t weather_get(const char* location, weather_t* weather)
{
    assert(location);
    /* 判断wifi是否连接成功 */
    if (!wifi_manager_is_connected())
    {
        ESP_LOGI(TAG, "weather cannot get due to false of wifi");
        return ESP_FAIL;
    }

    char local_url[256] = { 0 };
    weather_url_concat(location, local_url);
    /* 发送请求 */
    weather_http_perform(local_url, HTTP_METHOD_GET);


}
#ifndef __WEATHER_H__
#define __WEATHER_H__

#include "esp_err.h"
#include <stdint.h>
#include "esp_http_client.h"
typedef struct {
    int8_t code;   //天气代码
    int8_t temp;
}weather_t;

typedef struct {
    int8_t high;
    int8_t low;
    int8_t code;
} weather_day_t;

typedef struct {
    weather_day_t day[3];
} weather_forecast_t;

extern weather_t weather;
extern weather_forecast_t weather_forecast;

/**
 *@brief 初始化weather所需的http协议
 *
 */
esp_err_t weather_http_init(void);

/**
 *@brief 发送weather的http请求
 *
 * @param url 请求url
 * @param method 请求方法
 */
esp_err_t weather_http_perform(const char* url, esp_http_client_method_t method);

/**
 * @brief 生成一个获取天气的url
 *
 * @param location 想要获取的天气的城市
 * @param url 生成后返回的url
 */
void weather_url_concat(const char* location, char* url);

/**
 * @brief 获取某地的天气、温度
 *
 * @param location
 * @param weather
 */
esp_err_t weather_get(const char* location, weather_t* weather, weather_forecast_t* weather_forecast);
#endif /* __WEATHER_H__ */

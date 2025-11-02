#ifndef __WIFI_MANAGER_H__
#define __WIFI_MANAGER_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_err.h"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

void wifi_manager_init_sta(void);

/**
 * @brief 判断wifi是否连接成功
 *
 * @return true
 * @return false
 */
bool wifi_manager_is_connected(void);

/**
 * @brief 获取wifi事件句柄
 *
 * @return EventGroupHandle_t
 */
EventGroupHandle_t wifi_manager_get_event_group(void);

/**
 *@brief wifi主动连接
 *
 */
void wifi_manager_connect(void);

#endif /* __WIFI_MANAGER_H__ */




#ifndef __TIME_MANAGER_H__
#define __TIME_MANAGER_H__

#include "wifi_manager.h"
#include <time.h>

/**
 * @brief 主动进行时间同步
 *
 */
esp_err_t time_manager_sntp_start(void);

/**
 *@brief 是否时间同步
 *
 * @return true
 * @return false
 */
bool time_manager_is_time_synced(void);

/**
 *@brief 初始化stnp
 * 
 */
void time_manager_init(void);

#endif /* __TIME_MANAGER_H__ */

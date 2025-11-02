#ifndef __LV_PORT_H__
#define __LV_PORT_H__

#include "lvgl.h"

_lock_t* lv_port_get_lvgl_lock(void);

void lv_port_init(void);

#endif /* __LV_PORT_H__ */

#ifndef __UI_DIVER_H__
#define __UI_DIVER_H__
#include <stdint.h>
#include "lvgl.h"
void lvgl_time_driver(void);

void weather_dirver_init();
void set_weather_unknow(lv_obj_t* parent);
void flush_weather_ui_data(void);
void weather_ui_bind_icons(void);
void flush_other_card_data(void);
#endif /* __UI_DIVER_H__ */

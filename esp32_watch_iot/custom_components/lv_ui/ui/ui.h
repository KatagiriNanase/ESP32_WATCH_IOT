#ifndef __UI_H__
#define __UI_H__

#include "lvgl.h"

typedef struct {
    lv_obj_t* root;
    lv_obj_t* hour;
    lv_obj_t* min;
    lv_obj_t* sec;
    lv_obj_t* cont_bg;
    lv_obj_t* clock_bg;
} ui_clock_t;
extern ui_clock_t ui_clock_handle;
void set_up_src_clock(void);

typedef struct {
    lv_obj_t* root;
    lv_obj_t* month;
    lv_obj_t* day;
    lv_obj_t* hour;
    lv_obj_t* min;
    lv_obj_t* img;
}ui_home_t;
extern ui_home_t ui_home_handle;
void set_up_src_home(lv_obj_t* parent);

typedef struct {
    lv_obj_t* root;   //tabview的screen
    lv_obj_t* ui_home_root;
    lv_obj_t* ui_weather_root;
}ui_tileviw_t;
extern ui_tileviw_t ui_tileviw;
extern lv_obj_t* root_screen;

typedef struct {
    lv_obj_t* root;
    lv_obj_t* weather_card_root;
    lv_obj_t* today_temp;
    lv_obj_t* today_temp2;
    lv_obj_t* today_icon;
    lv_obj_t* today_icon2;
    lv_obj_t* today_label;
    lv_obj_t* tomorrow_temp;
    lv_obj_t* tomorrow_icon;
    lv_obj_t* tomorrow_label;
    lv_obj_t* after_temp;
    lv_obj_t* after_icon;
    lv_obj_t* after_label;
    lv_obj_t* scroll_label;
}ui_weather_t;
extern ui_weather_t ui_weather_handle;
void setup_src_weather(lv_obj_t* parent);
void set_up_src_tileview();
#endif /* __UI_H__ */




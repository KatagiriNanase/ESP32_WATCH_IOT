#include "ui.h"

ui_tileviw_t ui_tileviw;
lv_obj_t* root_screen;

void set_up_src_tileview(void)
{
    root_screen = lv_obj_create(NULL);
    ui_tileviw.root = lv_tileview_create(root_screen);
    lv_obj_set_scrollbar_mode(ui_tileviw.root, LV_SCROLLBAR_MODE_OFF);
    ui_tileviw.ui_home_root = lv_tileview_add_tile(ui_tileviw.root, 0, 0, LV_DIR_RIGHT);
    set_up_src_home(ui_tileviw.ui_home_root);

    ui_tileviw.ui_weather_root = lv_tileview_add_tile(ui_tileviw.root, 1, 0, LV_DIR_RIGHT | LV_DIR_LEFT);
    setup_src_weather(ui_tileviw.ui_weather_root);

}
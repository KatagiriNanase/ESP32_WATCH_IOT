#include "ui.h"

LV_FONT_DECLARE(lv_font_Acme_Regular_53)

ui_clock_t ui_clock_handle;

static void ui_clock_event_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        /* 切换屏幕 */
        set_up_src_tileview();
        lv_scr_load(root_screen);
        /* 释放本屏幕 */
        lv_obj_del(ui_clock_handle.root);
    }
}

void set_up_src_clock(void)
{
    /* 设置背景 */
    ui_clock_handle.root = lv_obj_create(NULL);
    ui_clock_handle.cont_bg = lv_obj_create(ui_clock_handle.root);
    lv_obj_set_size(ui_clock_handle.cont_bg, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(ui_clock_handle.cont_bg, lv_color_black(), 0);
    lv_obj_set_style_border_width(ui_clock_handle.cont_bg, 0, 0);
    lv_obj_add_flag(ui_clock_handle.cont_bg, LV_OBJ_FLAG_EVENT_BUBBLE);

    /* 设置clock的时分秒 */
    //设置透明cont
    ui_clock_handle.clock_bg = lv_obj_create(ui_clock_handle.cont_bg);
    lv_obj_set_size(ui_clock_handle.clock_bg, lv_pct(80), lv_pct(50));
    lv_obj_align(ui_clock_handle.clock_bg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(ui_clock_handle.clock_bg, LV_OPA_0, 0);
    lv_obj_set_style_border_width(ui_clock_handle.clock_bg, 0, 0);
    lv_obj_add_flag(ui_clock_handle.clock_bg, LV_OBJ_FLAG_EVENT_BUBBLE);

    //设置时分秒位置
    ui_clock_handle.hour = lv_label_create(ui_clock_handle.clock_bg);
    lv_label_set_text(ui_clock_handle.hour, "00");
    lv_obj_align(ui_clock_handle.hour, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_text_color(ui_clock_handle.hour, lv_color_white(), 0);
    lv_obj_set_style_text_font(ui_clock_handle.hour, &lv_font_Acme_Regular_53, 0);
    lv_obj_set_style_pad_hor(ui_clock_handle.hour, -10, 0);
    lv_obj_set_style_pad_ver(ui_clock_handle.hour, -20, 0);

    ui_clock_handle.min = lv_label_create(ui_clock_handle.clock_bg);
    lv_label_set_text(ui_clock_handle.min, "00");
    lv_obj_align(ui_clock_handle.min, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_color(ui_clock_handle.min, lv_color_white(), 0);
    lv_obj_set_style_text_font(ui_clock_handle.min, &lv_font_Acme_Regular_53, 0);

    ui_clock_handle.sec = lv_label_create(ui_clock_handle.clock_bg);
    lv_label_set_text(ui_clock_handle.sec, "00");
    lv_obj_set_style_text_font(ui_clock_handle.sec, &lv_font_Acme_Regular_53, 0);
    lv_obj_set_style_text_color(ui_clock_handle.sec, lv_color_white(), 0);
    lv_obj_align(ui_clock_handle.sec, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_set_style_pad_hor(ui_clock_handle.sec, -10, 0);
    lv_obj_set_style_pad_ver(ui_clock_handle.sec, -10, 0);

    /* 添加界面事件 */
    lv_obj_add_event_cb(ui_clock_handle.root, ui_clock_event_cb, LV_EVENT_CLICKED, NULL);

    lv_screen_load(ui_clock_handle.root);
}
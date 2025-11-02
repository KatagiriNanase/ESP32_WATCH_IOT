#include "ui.h"
#include <time.h>
extern struct tm* now_tm;
ui_home_t ui_home_handle;
LV_IMG_DECLARE(wallpaper)
LV_FONT_DECLARE(lv_font_Acme_Regular_28)
LV_FONT_DECLARE(lv_font_Acme_Regular_16)

void set_up_src_home(lv_obj_t* parent)
{
    ui_home_handle.root = parent;
    lv_obj_clear_flag(ui_home_handle.root, LV_OBJ_FLAG_SCROLLABLE);
    /* 设置img */
    ui_home_handle.img = lv_img_create(ui_home_handle.root);
    lv_img_set_src(ui_home_handle.img, &wallpaper);
    lv_obj_set_pos(ui_home_handle.img, 0, 0);

    /* 设置个性化线条 */
    //Write codes home_line_1
    lv_obj_t* ui_home_line_1 = lv_line_create(ui_home_handle.root);
    lv_obj_set_pos(ui_home_line_1, 166, 14);
    lv_obj_set_size(ui_home_line_1, 37, 108);
    static lv_point_precise_t home_line_1[] = { {35, 90},{0, 0} };
    lv_line_set_points(ui_home_line_1, home_line_1, 2);

    //Write style for home_line_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui_home_line_1, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui_home_line_1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui_home_line_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui_home_line_1, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes home_line_2
    lv_obj_t* ui_home_line_2 = lv_line_create(ui_home_handle.root);
    lv_obj_set_pos(ui_home_line_2, 208, 128);
    lv_obj_set_size(ui_home_line_2, 37, 108);
    static lv_point_precise_t home_line_2[] = { {34, 80},{0, 0} };
    lv_line_set_points(ui_home_line_2, home_line_2, 2);

    //Write style for home_line_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui_home_line_2, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui_home_line_2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui_home_line_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui_home_line_2, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes home_line_3
    lv_obj_t* ui_home_line_3 = lv_line_create(ui_home_handle.root);
    lv_obj_set_pos(ui_home_line_3, 42, 13);
    lv_obj_set_size(ui_home_line_3, 38, 95);
    static lv_point_precise_t home_line_3[] = { {30, 0},{0, 90} };
    lv_line_set_points(ui_home_line_3, home_line_3, 2);

    //Write style for home_line_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui_home_line_3, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui_home_line_3, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui_home_line_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui_home_line_3, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes home_line_4
    lv_obj_t* ui_home_line_4 = lv_line_create(ui_home_handle.root);
    lv_obj_set_pos(ui_home_line_4, 3, 129);
    lv_obj_set_size(ui_home_line_4, 38, 95);
    static lv_point_precise_t home_line_4[] = { {33, 0},{0, 90} };
    lv_line_set_points(ui_home_line_4, home_line_4, 2);

    //Write style for home_line_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui_home_line_4, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui_home_line_4, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui_home_line_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui_home_line_4, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    /* 设置数字时钟 */
//  //Write codes home_min
    ui_home_handle.min = lv_label_create(ui_home_handle.root);
    lv_obj_set_pos(ui_home_handle.min, 121, 240);
    lv_obj_set_size(ui_home_handle.min, 44, 25);
    lv_label_set_text_fmt(ui_home_handle.min, "%02d",now_tm->tm_min);
    lv_label_set_long_mode(ui_home_handle.min, LV_LABEL_LONG_WRAP);

    //Write style for home_min, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui_home_handle.min, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_home_handle.min, lv_color_hex(0xfefefe), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_home_handle.min, &lv_font_Acme_Regular_28, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_home_handle.min, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_home_handle.min, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_home_handle.min, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes home_hour
    ui_home_handle.hour = lv_label_create(ui_home_handle.root);
    lv_obj_set_pos(ui_home_handle.hour, 73, 240);
    lv_obj_set_size(ui_home_handle.hour, 44, 26);
    lv_label_set_text_fmt(ui_home_handle.hour, "%02d",now_tm->tm_hour);
    lv_label_set_long_mode(ui_home_handle.hour, LV_LABEL_LONG_WRAP);

    //Write style for home_hour, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui_home_handle.hour, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_home_handle.hour, lv_color_hex(0xfefefe), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_home_handle.hour, &lv_font_Acme_Regular_28, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_home_handle.hour, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_home_handle.hour, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes home_month
    ui_home_handle.month = lv_label_create(ui_home_handle.root);
    lv_obj_set_pos(ui_home_handle.month, 23, 108);
    lv_obj_set_size(ui_home_handle.month, 31, 21);
    lv_label_set_text_fmt(ui_home_handle.month, "%02d",now_tm->tm_mon+1);
    lv_label_set_long_mode(ui_home_handle.month, LV_LABEL_LONG_WRAP);

    //Write style for home_hour, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui_home_handle.month, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_home_handle.month, lv_color_hex(0xfefefe), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_home_handle.month, &lv_font_Acme_Regular_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_home_handle.month, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_home_handle.month, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_home_handle.month, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes home_day
    ui_home_handle.day = lv_label_create(ui_home_handle.root);
    lv_obj_set_pos(ui_home_handle.day, 189, 108);
    lv_obj_set_size(ui_home_handle.day, 31, 21);
    lv_label_set_text_fmt(ui_home_handle.day, "%02d",now_tm->tm_mday);
    lv_label_set_long_mode(ui_home_handle.day, LV_LABEL_LONG_WRAP);

    //Write style for home_day, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui_home_handle.day, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_home_handle.day, lv_color_hex(0xfefefe), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_home_handle.day, &lv_font_Acme_Regular_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_home_handle.day, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_home_handle.day, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_home_handle.day, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}
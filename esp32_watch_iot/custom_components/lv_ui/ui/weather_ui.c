#include "ui.h"
#include "ui_diver.h"

ui_weather_t ui_weather_handle;
LV_FONT_DECLARE(GenJyuuGothic_Normal_2)
LV_IMG_DECLARE(chisa)
LV_IMG_DECLARE(mask)
LV_FONT_DECLARE(lv_font_Acme_Regular_28)
LV_FONT_DECLARE(pixel_font)
void setup_src_weather(lv_obj_t* parent)
{
    ui_weather_handle.root = parent;

    /* 设置背景 */
    lv_obj_t* cont_bg = lv_obj_create(parent);
    lv_obj_set_size(cont_bg, 240, 280);
    lv_obj_set_style_bg_color(cont_bg, lv_color_black(), 0);
    lv_obj_set_style_border_width(cont_bg, 0, 0);
    lv_obj_set_style_pad_all(cont_bg, 15, 0);
    lv_obj_set_flex_flow(cont_bg, LV_FLEX_FLOW_COLUMN);
    lv_obj_clear_flag(cont_bg, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(cont_bg, LV_SCROLLBAR_MODE_OFF);

    /* 设置天气卡片 */
    lv_obj_t* weather_card = lv_obj_create(cont_bg);
    ui_weather_handle.weather_card_root = weather_card;
    lv_obj_set_flex_flow(weather_card, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_pad_row(weather_card, 0, 0);
    lv_obj_set_size(weather_card, lv_pct(100), lv_pct(60));
    lv_obj_set_style_radius(weather_card, 20, 0);
    lv_obj_set_style_border_width(weather_card, 0, 0);
    lv_obj_set_style_pad_all(weather_card, 0, 0);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_radius(&style, 5);

    /*Make a gradient*/
    lv_style_set_bg_opa(&style, LV_OPA_COVER);
    static lv_grad_dsc_t grad;
    grad.dir = LV_GRAD_DIR_VER;
    grad.stops_count = 2;
    grad.stops[0].color = lv_color_hex(0x273046);
    grad.stops[0].opa = LV_OPA_COVER;
    grad.stops[1].color = lv_color_hex(0x1c1b24);
    grad.stops[1].opa = LV_OPA_COVER;
    /*Shift the gradient to the bottom*/
    grad.stops[0].frac = 0;
    grad.stops[1].frac = 125;
    lv_style_set_bg_grad(&style, &grad);

    lv_obj_add_style(weather_card, &style, 0);

    lv_obj_t* other_card = lv_obj_create(cont_bg);
    lv_obj_set_size(other_card, lv_pct(100), lv_pct(40));
    lv_obj_set_style_radius(other_card, 20, 0);
    lv_obj_set_style_border_width(other_card, 0, 0);
    lv_obj_set_style_pad_all(other_card, 0, 0);
    lv_obj_set_style_bg_opa(other_card, LV_OPA_TRANSP, 0);

    /* ========= 上半部分 (今日温度 + 图标) ========= */
    lv_obj_t* row_top = lv_obj_create(weather_card);
    lv_obj_set_size(row_top, lv_pct(100), lv_pct(50));
    lv_obj_set_flex_flow(row_top, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row_top, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(row_top, LV_OPA_0, 0);
    lv_obj_set_style_border_width(row_top, 0, 0);
    lv_obj_clear_flag(row_top, LV_OBJ_FLAG_SCROLLABLE);
    // lv_obj_set_style_pad_all(row_top, 10, 0);

    ui_weather_handle.today_temp = lv_label_create(row_top);
    // lv_obj_set_style_margin_top(today_temp, -15, 0);     //把度数往上移
    lv_label_set_text(ui_weather_handle.today_temp, "16°");
    lv_obj_set_style_text_color(ui_weather_handle.today_temp, lv_color_white(), 0);
    lv_obj_set_style_text_font(ui_weather_handle.today_temp, &lv_font_montserrat_34, 0);

    /* 今日气温的图标 */
    ui_weather_handle.today_icon = lv_img_create(row_top);
    // lv_img_set_src(ui_weather_handle.today_icon, "/img/icon/sunny.png");

    /* ========= 下半部分 (未来温度 + 图标) ========= */
    lv_obj_t* row_bottom = lv_obj_create(weather_card);
    lv_obj_set_size(row_bottom, lv_pct(100), lv_pct(50));
    lv_obj_set_flex_flow(row_bottom, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row_bottom, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(row_bottom, LV_OPA_0, 0);
    lv_obj_set_style_border_width(row_bottom, 0, 0);
    lv_obj_set_style_pad_all(row_bottom, 0, 0);
    /* 设置天气cont通用样式 */
    static lv_style_t style2;
    lv_style_init(&style2);
    lv_style_set_size(&style2, lv_pct(30), lv_pct(100));
    lv_style_set_border_width(&style2, 0);
    lv_style_set_bg_opa(&style2, LV_OPA_0);
    lv_style_set_text_font(&style2, &GenJyuuGothic_Normal_2);
    lv_style_set_text_color(&style2, lv_color_white());

    /* ---------- 设置今日天气 ---------- */
    /* 设置今天小天气 */
    lv_obj_t* bottom_cont_day1 = lv_obj_create(row_bottom);
    lv_obj_add_style(bottom_cont_day1, &style2, 0);
    lv_obj_clear_flag(bottom_cont_day1, LV_OBJ_FLAG_SCROLLABLE);
    ui_weather_handle.today_label = lv_label_create(bottom_cont_day1);
    // lv_label_set_text(label1, "今天");
    lv_obj_align(ui_weather_handle.today_label, LV_ALIGN_TOP_MID, 0, -10);
    /* 放置图标 */
    ui_weather_handle.today_icon2 = lv_img_create(bottom_cont_day1);
    // lv_img_set_src(ui_weather_handle.today_icon2, "/img/icon/sunny.png");
    // lv_img_set_zoom(ui_weather_handle.today_icon2, 160);
    lv_obj_align(ui_weather_handle.today_icon2, LV_ALIGN_CENTER, 0, 0);
    /* 设置温度 */
    ui_weather_handle.today_temp2 = lv_label_create(bottom_cont_day1);
    // lv_label_set_text_fmt(ui_weather_handle.today_temp2, "26°");
    lv_obj_align(ui_weather_handle.today_temp2, LV_ALIGN_BOTTOM_MID, 4, 10);

    /* ---------- 设置明日天气 ---------- */
    /* 设置今天小天气 */
    lv_obj_t* bottom_cont_day2 = lv_obj_create(row_bottom);
    lv_obj_add_style(bottom_cont_day2, &style2, 0);
    lv_obj_clear_flag(bottom_cont_day2, LV_OBJ_FLAG_SCROLLABLE);
    ui_weather_handle.tomorrow_label = lv_label_create(bottom_cont_day2);
    // lv_label_set_text(label2, "明天");
    lv_obj_align(ui_weather_handle.tomorrow_label, LV_ALIGN_TOP_MID, 0, -10);
    /* 放置图标 */
    ui_weather_handle.tomorrow_icon = lv_img_create(bottom_cont_day2);
    // lv_img_set_src(ui_weather_handle.tomorrow_icon, "/img/icon/sunny.png");
    // lv_img_set_zoom(ui_weather_handle.tomorrow_icon, 160);
    lv_obj_align(ui_weather_handle.tomorrow_icon, LV_ALIGN_CENTER, 0, 0);
    /* 设置温度 */
    ui_weather_handle.tomorrow_temp = lv_label_create(bottom_cont_day2);
    // lv_label_set_text_fmt(ui_weather_handle.tomorrow_temp, "26°");
    lv_obj_align(ui_weather_handle.tomorrow_temp, LV_ALIGN_BOTTOM_MID, 4, 10);

    /* ---------- 设置后天天气 ---------- */
    /* 设置今天小天气 */
    lv_obj_t* bottom_cont_day3 = lv_obj_create(row_bottom);
    lv_obj_add_style(bottom_cont_day3, &style2, 0);
    lv_obj_clear_flag(bottom_cont_day3, LV_OBJ_FLAG_SCROLLABLE);
    ui_weather_handle.after_label = lv_label_create(bottom_cont_day3);
    // lv_label_set_text(label3, "后天");
    lv_obj_align(ui_weather_handle.after_label, LV_ALIGN_TOP_MID, 0, -10);
    /* 放置图标 */
    ui_weather_handle.after_icon = lv_img_create(bottom_cont_day3);
    // lv_img_set_src(ui_weather_handle.after_icon, "/img/icon/sunny.png");
    // lv_img_set_zoom(ui_weather_handle.after_icon, 160);
    lv_obj_align(ui_weather_handle.after_icon, LV_ALIGN_CENTER, 0, 0);
    /* 设置温度 */
    ui_weather_handle.after_temp = lv_label_create(bottom_cont_day3);
    // lv_label_set_text_fmt(ui_weather_handle.after_temp, "26°");
    lv_obj_align(ui_weather_handle.after_temp, LV_ALIGN_BOTTOM_MID, 4, 10);

    weather_ui_bind_icons();

    /* 设置other_card背景 */
    lv_obj_set_style_bg_img_src(other_card, &chisa, 0);
    lv_obj_set_style_bg_image_opa(other_card, LV_OPA_70, 0);

    /* 设置小蒙版 */
    static lv_style_t other_card_style;
    lv_style_init(&other_card_style);
    lv_style_set_bg_opa(&other_card_style, LV_OPA_40);
    lv_style_set_border_width(&other_card_style, 1);
    lv_style_set_border_color(&other_card_style, lv_color_black());
    lv_style_set_pad_all(&other_card_style, 0);
    lv_style_set_bg_color(&other_card_style, lv_color_hex(0x32253c));
    lv_style_set_text_font(&other_card_style, &GenJyuuGothic_Normal_2);
    lv_style_set_text_color(&other_card_style, lv_color_white());

    /* 通用样式：去除border pad 背景透明 */
    static lv_style_t common_style;
    lv_style_init(&common_style);
    lv_style_set_border_width(&common_style, 0);
    lv_style_set_bg_opa(&common_style, 0);
    lv_style_set_pad_all(&common_style, 0);

    lv_obj_t* sub_cont = lv_obj_create(other_card);
    lv_obj_set_size(sub_cont, lv_pct(90), lv_pct(90));
    lv_obj_set_align(sub_cont, LV_ALIGN_CENTER);
    lv_obj_add_style(sub_cont, &other_card_style, 0);

    // /* 添加一个tile */
    lv_obj_t* other_tileview = lv_tileview_create(sub_cont);
    lv_obj_set_style_bg_opa(other_tileview, LV_OPA_0, 0);
    lv_obj_clear_flag(other_tileview, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_clear_flag(other_tileview, LV_OBJ_FLAG_SCROLL_CHAIN);

    lv_obj_t* sub_tile1 = lv_tileview_add_tile(other_tileview, 0, 0, LV_DIR_RIGHT);
    lv_obj_set_style_pad_all(sub_tile1, 0, 0);
    lv_obj_set_size(sub_tile1, lv_obj_get_width(sub_cont), lv_obj_get_height(sub_cont));

    lv_obj_t* sub_tile1_cont = lv_obj_create(sub_tile1);
    lv_obj_set_size(sub_tile1_cont, lv_obj_get_width(sub_cont), lv_obj_get_height(sub_cont));
    lv_obj_set_pos(sub_tile1_cont, 0, 0);
    lv_obj_add_style(sub_tile1_cont, &common_style, 0);
    lv_obj_clear_flag(sub_tile1_cont, LV_OBJ_FLAG_SCROLLABLE);

    // /* 设置俩小团子 */
    lv_obj_t* miside_img = lv_img_create(sub_tile1_cont);
    lv_img_set_src(miside_img, "/img/icon/miside.png");
    lv_obj_align(miside_img, LV_ALIGN_BOTTOM_LEFT, 10, -5);

    lv_obj_t* jiabei_img = lv_img_create(sub_tile1_cont);
    lv_img_set_src(jiabei_img, "/img/icon/jiabei.png");
    lv_obj_align(jiabei_img, LV_ALIGN_BOTTOM_RIGHT, -10, -5);

    // /* 设置温湿度 */
    static lv_style_t font_style;
    lv_style_init(&font_style);
    lv_style_set_text_font(&font_style, &pixel_font);
    lv_style_set_text_color(&font_style, lv_color_white());
    ui_weather_handle.scroll_label = lv_label_create(sub_tile1_cont);
    lv_obj_set_width(ui_weather_handle.scroll_label, lv_pct(90));
    lv_obj_add_style(ui_weather_handle.scroll_label, &font_style, 0);
    lv_label_set_long_mode(ui_weather_handle.scroll_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_align(ui_weather_handle.scroll_label, LV_ALIGN_TOP_MID, 0, 5);
    lv_label_set_text_fmt(ui_weather_handle.scroll_label, "室温:--°C 湿度:-- 今天团子也给你加油ヾ(*´v ˋ*)ﾉ");
    /* 设置小彩蛋 */
    lv_obj_t* sub_tile2 = lv_tileview_add_tile(other_tileview, 1, 0, LV_DIR_RIGHT | LV_DIR_LEFT);
    lv_obj_set_style_pad_all(sub_tile2, 0, 0);
    lv_obj_set_size(sub_tile2, lv_obj_get_width(sub_cont), lv_obj_get_height(sub_cont));
    lv_obj_clear_flag(sub_tile2, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_t* mask_img = lv_img_create(sub_tile2);
    lv_img_set_src(mask_img, &mask);
    lv_obj_align(mask_img, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_angle(mask_img, -300); // 45.0°

    /* 主动刷新一次屏幕 */
    flush_weather_ui_data();
    flush_other_card_data();

}
/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
// #include "gui_guider.h"
// #include "events_init.h"
// #include "widgets_init.h"
// #include "custom.h"



int start_digital_clock_1_min_value = 59;
int start_digital_clock_1_hour_value = 23;
int start_digital_clock_1_sec_value = 59;
void setup_scr_start(lv_ui *ui)
{
    //Write codes start
    ui->start = lv_obj_create(NULL);
    lv_obj_set_size(ui->start, 320, 240);
    lv_obj_set_scrollbar_mode(ui->start, LV_SCROLLBAR_MODE_OFF);

    //Write style for start, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->start, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->start, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->start, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes start_btn_setting
    ui->start_btn_setting = lv_button_create(ui->start);
    lv_obj_set_pos(ui->start_btn_setting, 249, 199);
    lv_obj_set_size(ui->start_btn_setting, 60, 30);
    ui->start_btn_setting_label = lv_label_create(ui->start_btn_setting);
    lv_label_set_text(ui->start_btn_setting_label, "" LV_SYMBOL_SETTINGS " ");
    lv_label_set_long_mode(ui->start_btn_setting_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->start_btn_setting_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->start_btn_setting, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->start_btn_setting_label, LV_PCT(100));

    //Write style for start_btn_setting, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->start_btn_setting, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->start_btn_setting, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->start_btn_setting, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->start_btn_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->start_btn_setting, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->start_btn_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->start_btn_setting, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->start_btn_setting, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->start_btn_setting, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->start_btn_setting, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes start_btn_usb
    ui->start_btn_usb = lv_button_create(ui->start);
    lv_obj_set_pos(ui->start_btn_usb, 130, 199);
    lv_obj_set_size(ui->start_btn_usb, 60, 30);
    ui->start_btn_usb_label = lv_label_create(ui->start_btn_usb);
    lv_label_set_text(ui->start_btn_usb_label, "" LV_SYMBOL_USB " ");
    lv_label_set_long_mode(ui->start_btn_usb_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->start_btn_usb_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->start_btn_usb, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->start_btn_usb_label, LV_PCT(100));

    //Write style for start_btn_usb, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->start_btn_usb, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->start_btn_usb, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->start_btn_usb, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->start_btn_usb, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->start_btn_usb, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->start_btn_usb, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->start_btn_usb, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->start_btn_usb, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->start_btn_usb, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->start_btn_usb, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes start_btn_sd
    ui->start_btn_sd = lv_button_create(ui->start);
    lv_obj_set_pos(ui->start_btn_sd, 9, 199);
    lv_obj_set_size(ui->start_btn_sd, 60, 30);
    ui->start_btn_sd_label = lv_label_create(ui->start_btn_sd);
    lv_label_set_text(ui->start_btn_sd_label, "" LV_SYMBOL_SD_CARD " ");
    lv_label_set_long_mode(ui->start_btn_sd_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->start_btn_sd_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->start_btn_sd, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->start_btn_sd_label, LV_PCT(100));

    //Write style for start_btn_sd, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->start_btn_sd, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->start_btn_sd, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->start_btn_sd, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->start_btn_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->start_btn_sd, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->start_btn_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->start_btn_sd, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->start_btn_sd, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->start_btn_sd, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->start_btn_sd, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes start_btn_ACMeter
    ui->start_btn_ACMeter = lv_button_create(ui->start);
    lv_obj_set_pos(ui->start_btn_ACMeter, 189, 104);
    lv_obj_set_size(ui->start_btn_ACMeter, 120, 30);
    ui->start_btn_ACMeter_label = lv_label_create(ui->start_btn_ACMeter);
    lv_label_set_text(ui->start_btn_ACMeter_label, "AC UI Meter " LV_SYMBOL_CHARGE " ");
    lv_label_set_long_mode(ui->start_btn_ACMeter_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->start_btn_ACMeter_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->start_btn_ACMeter, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->start_btn_ACMeter_label, LV_PCT(100));

    //Write style for start_btn_ACMeter, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->start_btn_ACMeter, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->start_btn_ACMeter, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->start_btn_ACMeter, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->start_btn_ACMeter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->start_btn_ACMeter, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->start_btn_ACMeter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->start_btn_ACMeter, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->start_btn_ACMeter, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->start_btn_ACMeter, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->start_btn_ACMeter, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes start_btn_HB
    ui->start_btn_HB = lv_button_create(ui->start);
    lv_obj_set_pos(ui->start_btn_HB, 9, 104);
    lv_obj_set_size(ui->start_btn_HB, 120, 30);
    lv_obj_add_flag(ui->start_btn_HB, LV_OBJ_FLAG_CHECKABLE);
    ui->start_btn_HB_label = lv_label_create(ui->start_btn_HB);
    lv_label_set_text(ui->start_btn_HB_label, "Hot Board " LV_SYMBOL_WARNING " ");
    lv_label_set_long_mode(ui->start_btn_HB_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->start_btn_HB_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->start_btn_HB, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->start_btn_HB_label, LV_PCT(100));

    //Write style for start_btn_HB, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->start_btn_HB, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->start_btn_HB, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->start_btn_HB, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->start_btn_HB, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->start_btn_HB, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->start_btn_HB, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->start_btn_HB, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->start_btn_HB, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->start_btn_HB, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->start_btn_HB, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes start_label_sd
    ui->start_label_sd = lv_label_create(ui->start);
    lv_obj_set_pos(ui->start_label_sd, 284, 9);
    lv_obj_set_size(ui->start_label_sd, 30, 30);
    lv_obj_add_flag(ui->start_label_sd, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui->start_label_sd, "" LV_SYMBOL_SD_CARD " ");
    lv_label_set_long_mode(ui->start_label_sd, LV_LABEL_LONG_WRAP);

    //Write style for start_label_sd, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->start_label_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->start_label_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->start_label_sd, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->start_label_sd, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->start_label_sd, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->start_label_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->start_label_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->start_label_sd, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->start_label_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->start_label_sd, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->start_label_sd, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->start_label_sd, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->start_label_sd, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->start_label_sd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes start_lable_Flash
    ui->start_lable_Flash = lv_label_create(ui->start);
    lv_obj_set_pos(ui->start_lable_Flash, 249, 9);
    lv_obj_set_size(ui->start_lable_Flash, 30, 30);
    lv_obj_add_flag(ui->start_lable_Flash, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui->start_lable_Flash, "" LV_SYMBOL_DRIVE " ");
    lv_label_set_long_mode(ui->start_lable_Flash, LV_LABEL_LONG_WRAP);

    //Write style for start_lable_Flash, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->start_lable_Flash, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->start_lable_Flash, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->start_lable_Flash, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->start_lable_Flash, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->start_lable_Flash, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->start_lable_Flash, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->start_lable_Flash, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->start_lable_Flash, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->start_lable_Flash, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->start_lable_Flash, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->start_lable_Flash, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->start_lable_Flash, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->start_lable_Flash, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->start_lable_Flash, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->start_lable_Flash, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->start_lable_Flash, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes start_digital_clock_1
    static bool start_digital_clock_1_timer_enabled = false;
    ui->start_digital_clock_1 = lv_label_create(ui->start);
    lv_obj_set_pos(ui->start_digital_clock_1, 49, 9);
    lv_obj_set_size(ui->start_digital_clock_1, 65, 30);
    lv_label_set_text(ui->start_digital_clock_1, "23:59:59");
    if (!start_digital_clock_1_timer_enabled) {
        lv_timer_create(start_digital_clock_1_timer, 1000, NULL);
        start_digital_clock_1_timer_enabled = true;
    }

    //Write style for start_digital_clock_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->start_digital_clock_1, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->start_digital_clock_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->start_digital_clock_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->start_digital_clock_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->start_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->start_digital_clock_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->start_digital_clock_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->start_digital_clock_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->start_digital_clock_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->start_digital_clock_1, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->start_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->start_digital_clock_1, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->start_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->start_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of start.


    //Update current screen layout.
    lv_obj_update_layout(ui->start);

    //Init events for screen.
    events_init_start(ui);
}

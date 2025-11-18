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
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



int start_digital_clock_1_min_value = 0;
int start_digital_clock_1_hour_value = 0;
int start_digital_clock_1_sec_value = 0;
void setup_scr_start(lv_ui *ui)
{
    //Write codes start
    ui->start = lv_obj_create(NULL);
    lv_obj_set_size(ui->start, 320, 240);
    lv_obj_set_scrollbar_mode(ui->start, LV_SCROLLBAR_MODE_OFF);
    lv_obj_remove_flag(ui->start, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(ui->start, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_remove_flag(ui->start, LV_OBJ_FLAG_CHECKABLE);

    //Write style for start, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->start, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->start, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->start, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes start_btn_setting
    ui->start_btn_setting = lv_button_create(ui->start);
    lv_obj_set_pos(ui->start_btn_setting, 250, 150);
    lv_obj_set_size(ui->start_btn_setting, 60, 50);
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
    lv_obj_set_style_radius(ui->start_btn_setting, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->start_btn_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->start_btn_setting, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->start_btn_setting, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->start_btn_setting, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->start_btn_setting, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes start_btn_USB
    ui->start_btn_USB = lv_button_create(ui->start);
    lv_obj_set_pos(ui->start_btn_USB, 130, 150);
    lv_obj_set_size(ui->start_btn_USB, 60, 50);
    ui->start_btn_USB_label = lv_label_create(ui->start_btn_USB);
    lv_label_set_text(ui->start_btn_USB_label, "" LV_SYMBOL_USB " ");
    lv_label_set_long_mode(ui->start_btn_USB_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->start_btn_USB_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->start_btn_USB, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->start_btn_USB_label, LV_PCT(100));

    //Write style for start_btn_USB, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->start_btn_USB, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->start_btn_USB, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->start_btn_USB, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->start_btn_USB, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->start_btn_USB, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->start_btn_USB, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->start_btn_USB, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->start_btn_USB, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->start_btn_USB, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->start_btn_USB, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes start_btn_FH
    ui->start_btn_FH = lv_button_create(ui->start);
    lv_obj_set_pos(ui->start_btn_FH, 10, 150);
    lv_obj_set_size(ui->start_btn_FH, 60, 50);
    ui->start_btn_FH_label = lv_label_create(ui->start_btn_FH);
    lv_label_set_text(ui->start_btn_FH_label, "" LV_SYMBOL_SD_CARD " ");
    lv_label_set_long_mode(ui->start_btn_FH_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->start_btn_FH_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->start_btn_FH, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->start_btn_FH_label, LV_PCT(100));

    //Write style for start_btn_FH, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->start_btn_FH, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->start_btn_FH, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->start_btn_FH, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->start_btn_FH, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->start_btn_FH, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->start_btn_FH, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->start_btn_FH, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->start_btn_FH, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->start_btn_FH, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->start_btn_FH, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes start_btn_AM
    ui->start_btn_AM = lv_button_create(ui->start);
    lv_obj_set_pos(ui->start_btn_AM, 190, 70);
    lv_obj_set_size(ui->start_btn_AM, 120, 50);
    ui->start_btn_AM_label = lv_label_create(ui->start_btn_AM);
    lv_label_set_text(ui->start_btn_AM_label, "AC Meter " LV_SYMBOL_CHARGE " ");
    lv_label_set_long_mode(ui->start_btn_AM_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->start_btn_AM_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->start_btn_AM, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->start_btn_AM_label, LV_PCT(100));

    //Write style for start_btn_AM, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->start_btn_AM, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->start_btn_AM, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->start_btn_AM, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->start_btn_AM, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->start_btn_AM, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->start_btn_AM, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->start_btn_AM, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->start_btn_AM, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->start_btn_AM, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->start_btn_AM, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes start_btn_HB
    ui->start_btn_HB = lv_button_create(ui->start);
    lv_obj_set_pos(ui->start_btn_HB, 10, 70);
    lv_obj_set_size(ui->start_btn_HB, 120, 50);
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
    lv_obj_set_style_radius(ui->start_btn_HB, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->start_btn_HB, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->start_btn_HB, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->start_btn_HB, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->start_btn_HB, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->start_btn_HB, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes start_label_eeprom
    ui->start_label_eeprom = lv_label_create(ui->start);
    lv_obj_set_pos(ui->start_label_eeprom, 295, 5);
    lv_obj_set_size(ui->start_label_eeprom, 20, 20);
    lv_obj_add_flag(ui->start_label_eeprom, LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_flag(ui->start_label_eeprom, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(ui->start_label_eeprom, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_remove_flag(ui->start_label_eeprom, LV_OBJ_FLAG_CHECKABLE);
    lv_label_set_text(ui->start_label_eeprom, "" LV_SYMBOL_SAVE " ");
    lv_label_set_long_mode(ui->start_label_eeprom, LV_LABEL_LONG_WRAP);

    //Write style for start_label_eeprom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->start_label_eeprom, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->start_label_eeprom, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->start_label_eeprom, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->start_label_eeprom, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->start_label_eeprom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->start_label_eeprom, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->start_label_eeprom, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->start_label_eeprom, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->start_label_eeprom, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->start_label_eeprom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->start_label_eeprom, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->start_label_eeprom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->start_label_eeprom, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->start_label_eeprom, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes start_label_flash
    ui->start_label_flash = lv_label_create(ui->start);
    lv_obj_set_pos(ui->start_label_flash, 258, 5);
    lv_obj_set_size(ui->start_label_flash, 20, 20);
    lv_obj_add_flag(ui->start_label_flash, LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_flag(ui->start_label_flash, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(ui->start_label_flash, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_remove_flag(ui->start_label_flash, LV_OBJ_FLAG_CHECKABLE);
    lv_label_set_text(ui->start_label_flash, "" LV_SYMBOL_DRIVE " ");
    lv_label_set_long_mode(ui->start_label_flash, LV_LABEL_LONG_WRAP);

    //Write style for start_label_flash, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->start_label_flash, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->start_label_flash, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->start_label_flash, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->start_label_flash, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->start_label_flash, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->start_label_flash, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->start_label_flash, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->start_label_flash, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->start_label_flash, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->start_label_flash, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->start_label_flash, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->start_label_flash, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->start_label_flash, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->start_label_flash, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->start_label_flash, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->start_label_flash, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes start_digital_clock_1
    static bool start_digital_clock_1_timer_enabled = false;
    ui->start_digital_clock_1 = lv_label_create(ui->start);
    lv_obj_set_pos(ui->start_digital_clock_1, 49, 5);
    lv_obj_set_size(ui->start_digital_clock_1, 70, 20);
    lv_obj_remove_flag(ui->start_digital_clock_1, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(ui->start_digital_clock_1, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_remove_flag(ui->start_digital_clock_1, LV_OBJ_FLAG_CHECKABLE);
    lv_label_set_text(ui->start_digital_clock_1, "0:00:00");
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
    lv_obj_set_style_pad_top(ui->start_digital_clock_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->start_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->start_digital_clock_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->start_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->start_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of start.


    //Update current screen layout.
    lv_obj_update_layout(ui->start);

    //Init events for screen.
    events_init_start(ui);
}

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



int Hot_Board_digital_clock_1_min_value = 0;
int Hot_Board_digital_clock_1_hour_value = 0;
int Hot_Board_digital_clock_1_sec_value = 0;
void setup_scr_Hot_Board(lv_ui *ui)
{
    //Write codes Hot_Board
    ui->Hot_Board = lv_obj_create(NULL);
    lv_obj_set_size(ui->Hot_Board, 320, 240);
    lv_obj_set_scrollbar_mode(ui->Hot_Board, LV_SCROLLBAR_MODE_OFF);

    //Write style for Hot_Board, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Hot_Board, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Hot_Board, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Hot_Board, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Hot_Board_btn_enter
    ui->Hot_Board_btn_enter = lv_button_create(ui->Hot_Board);
    lv_obj_set_pos(ui->Hot_Board_btn_enter, 276, 201);
    lv_obj_set_size(ui->Hot_Board_btn_enter, 20, 20);
    ui->Hot_Board_btn_enter_label = lv_label_create(ui->Hot_Board_btn_enter);
    lv_label_set_text(ui->Hot_Board_btn_enter_label, "" LV_SYMBOL_OK " ");
    lv_label_set_long_mode(ui->Hot_Board_btn_enter_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Hot_Board_btn_enter_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Hot_Board_btn_enter, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Hot_Board_btn_enter_label, LV_PCT(100));

    //Write style for Hot_Board_btn_enter, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Hot_Board_btn_enter, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Hot_Board_btn_enter, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Hot_Board_btn_enter, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Hot_Board_btn_enter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Hot_Board_btn_enter, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Hot_Board_btn_enter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Hot_Board_btn_enter, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Hot_Board_btn_enter, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Hot_Board_btn_enter, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Hot_Board_btn_enter, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Hot_Board_spinbox_1
    ui->Hot_Board_spinbox_1 = lv_spinbox_create(ui->Hot_Board);
    lv_obj_set_pos(ui->Hot_Board_spinbox_1, 119, 189);
    lv_obj_set_width(ui->Hot_Board_spinbox_1, 80);
    lv_obj_set_height(ui->Hot_Board_spinbox_1, 45);
    lv_spinbox_set_digit_format(ui->Hot_Board_spinbox_1, 5, 3);
    lv_spinbox_set_range(ui->Hot_Board_spinbox_1, -99999, 99999);
    int32_t Hot_Board_spinbox_1_h = lv_obj_get_height(ui->Hot_Board_spinbox_1);
    ui->Hot_Board_spinbox_1_btn_plus = lv_button_create(ui->Hot_Board);
    lv_obj_set_size(ui->Hot_Board_spinbox_1_btn_plus, Hot_Board_spinbox_1_h, Hot_Board_spinbox_1_h);
    lv_obj_align_to(ui->Hot_Board_spinbox_1_btn_plus, ui->Hot_Board_spinbox_1, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_image_src(ui->Hot_Board_spinbox_1_btn_plus, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(ui->Hot_Board_spinbox_1_btn_plus, lv_Hot_Board_spinbox_1_increment_event_cb, LV_EVENT_ALL, NULL);
    ui->Hot_Board_spinbox_1_btn_minus = lv_button_create(ui->Hot_Board);
    lv_obj_set_size(ui->Hot_Board_spinbox_1_btn_minus, Hot_Board_spinbox_1_h, Hot_Board_spinbox_1_h);
    lv_obj_align_to(ui->Hot_Board_spinbox_1_btn_minus, ui->Hot_Board_spinbox_1, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_bg_image_src(ui->Hot_Board_spinbox_1_btn_minus, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(ui->Hot_Board_spinbox_1_btn_minus, lv_Hot_Board_spinbox_1_decrement_event_cb, LV_EVENT_ALL, NULL);

    //Write style for Hot_Board_spinbox_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Hot_Board_spinbox_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Hot_Board_spinbox_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Hot_Board_spinbox_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Hot_Board_spinbox_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Hot_Board_spinbox_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Hot_Board_spinbox_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Hot_Board_spinbox_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Hot_Board_spinbox_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Hot_Board_spinbox_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Hot_Board_spinbox_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Hot_Board_spinbox_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Hot_Board_spinbox_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Hot_Board_spinbox_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Hot_Board_spinbox_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Hot_Board_spinbox_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Hot_Board_spinbox_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Hot_Board_spinbox_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for Hot_Board_spinbox_1, Part: LV_PART_CURSOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->Hot_Board_spinbox_1, lv_color_hex(0xffffff), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Hot_Board_spinbox_1, &lv_font_montserratMedium_16, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Hot_Board_spinbox_1, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Hot_Board_spinbox_1, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Hot_Board_spinbox_1, lv_color_hex(0xff0096), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Hot_Board_spinbox_1, LV_GRAD_DIR_NONE, LV_PART_CURSOR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_Hot_Board_spinbox_1_extra_btns_main_default
    static lv_style_t style_Hot_Board_spinbox_1_extra_btns_main_default;
    ui_init_style(&style_Hot_Board_spinbox_1_extra_btns_main_default);

    lv_style_set_text_color(&style_Hot_Board_spinbox_1_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_Hot_Board_spinbox_1_extra_btns_main_default, &lv_font_montserratMedium_16);
    lv_style_set_text_opa(&style_Hot_Board_spinbox_1_extra_btns_main_default, 255);
    lv_style_set_bg_opa(&style_Hot_Board_spinbox_1_extra_btns_main_default, 0);
    lv_style_set_border_width(&style_Hot_Board_spinbox_1_extra_btns_main_default, 0);
    lv_style_set_radius(&style_Hot_Board_spinbox_1_extra_btns_main_default, 5);
    lv_style_set_shadow_width(&style_Hot_Board_spinbox_1_extra_btns_main_default, 0);
    lv_obj_add_style(ui->Hot_Board_spinbox_1_btn_plus, &style_Hot_Board_spinbox_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->Hot_Board_spinbox_1_btn_minus, &style_Hot_Board_spinbox_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_PRESSED for &style_Hot_Board_spinbox_1_extra_btns_main_pressed
    static lv_style_t style_Hot_Board_spinbox_1_extra_btns_main_pressed;
    ui_init_style(&style_Hot_Board_spinbox_1_extra_btns_main_pressed);

    lv_style_set_text_color(&style_Hot_Board_spinbox_1_extra_btns_main_pressed, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_Hot_Board_spinbox_1_extra_btns_main_pressed, &lv_font_montserratMedium_16);
    lv_style_set_text_opa(&style_Hot_Board_spinbox_1_extra_btns_main_pressed, 255);
    lv_style_set_bg_opa(&style_Hot_Board_spinbox_1_extra_btns_main_pressed, 0);
    lv_style_set_border_width(&style_Hot_Board_spinbox_1_extra_btns_main_pressed, 1);
    lv_style_set_border_opa(&style_Hot_Board_spinbox_1_extra_btns_main_pressed, 255);
    lv_style_set_border_color(&style_Hot_Board_spinbox_1_extra_btns_main_pressed, lv_color_hex(0x2195f6));
    lv_style_set_border_side(&style_Hot_Board_spinbox_1_extra_btns_main_pressed, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_Hot_Board_spinbox_1_extra_btns_main_pressed, 5);
    lv_style_set_shadow_width(&style_Hot_Board_spinbox_1_extra_btns_main_pressed, 0);
    lv_obj_add_style(ui->Hot_Board_spinbox_1_btn_plus, &style_Hot_Board_spinbox_1_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_add_style(ui->Hot_Board_spinbox_1_btn_minus, &style_Hot_Board_spinbox_1_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write codes Hot_Board_sw_start
    ui->Hot_Board_sw_start = lv_switch_create(ui->Hot_Board);
    lv_obj_set_pos(ui->Hot_Board_sw_start, 14, 201);
    lv_obj_set_size(ui->Hot_Board_sw_start, 40, 20);

    //Write style for Hot_Board_sw_start, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Hot_Board_sw_start, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Hot_Board_sw_start, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Hot_Board_sw_start, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Hot_Board_sw_start, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Hot_Board_sw_start, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Hot_Board_sw_start, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for Hot_Board_sw_start, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->Hot_Board_sw_start, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->Hot_Board_sw_start, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->Hot_Board_sw_start, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->Hot_Board_sw_start, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for Hot_Board_sw_start, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Hot_Board_sw_start, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Hot_Board_sw_start, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Hot_Board_sw_start, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Hot_Board_sw_start, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Hot_Board_sw_start, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes Hot_Board_label_C
    ui->Hot_Board_label_C = lv_label_create(ui->Hot_Board);
    lv_obj_set_pos(ui->Hot_Board_label_C, 269, 84);
    lv_obj_set_size(ui->Hot_Board_label_C, 45, 60);
    lv_obj_remove_flag(ui->Hot_Board_label_C, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(ui->Hot_Board_label_C, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_remove_flag(ui->Hot_Board_label_C, LV_OBJ_FLAG_CHECKABLE);
    lv_label_set_text(ui->Hot_Board_label_C, "C");
    lv_label_set_long_mode(ui->Hot_Board_label_C, LV_LABEL_LONG_WRAP);

    //Write style for Hot_Board_label_C, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Hot_Board_label_C, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Hot_Board_label_C, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Hot_Board_label_C, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Hot_Board_label_C, &lv_font_montserratMedium_60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Hot_Board_label_C, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Hot_Board_label_C, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Hot_Board_label_C, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Hot_Board_label_C, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Hot_Board_label_C, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Hot_Board_label_C, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Hot_Board_label_C, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Hot_Board_label_C, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Hot_Board_label_C, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Hot_Board_label_C, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Hot_Board_label_setv_value
    ui->Hot_Board_label_setv_value = lv_label_create(ui->Hot_Board);
    lv_obj_set_pos(ui->Hot_Board_label_setv_value, 119, 154);
    lv_obj_set_size(ui->Hot_Board_label_setv_value, 80, 20);
    lv_obj_remove_flag(ui->Hot_Board_label_setv_value, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(ui->Hot_Board_label_setv_value, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_remove_flag(ui->Hot_Board_label_setv_value, LV_OBJ_FLAG_CHECKABLE);
    lv_label_set_text(ui->Hot_Board_label_setv_value, "-000.0");
    lv_label_set_long_mode(ui->Hot_Board_label_setv_value, LV_LABEL_LONG_WRAP);

    //Write style for Hot_Board_label_setv_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Hot_Board_label_setv_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Hot_Board_label_setv_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Hot_Board_label_setv_value, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Hot_Board_label_setv_value, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Hot_Board_label_setv_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Hot_Board_label_setv_value, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Hot_Board_label_setv_value, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Hot_Board_label_setv_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Hot_Board_label_setv_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Hot_Board_label_setv_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Hot_Board_label_setv_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Hot_Board_label_setv_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Hot_Board_label_setv_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Hot_Board_label_setv_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Hot_Board_label_setv_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Hot_Board_label_setv_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Hot_Board_label_setv_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Hot_Board_label_temp
    ui->Hot_Board_label_temp = lv_label_create(ui->Hot_Board);
    lv_obj_set_pos(ui->Hot_Board_label_temp, 49, 84);
    lv_obj_set_size(ui->Hot_Board_label_temp, 220, 60);
    lv_obj_remove_flag(ui->Hot_Board_label_temp, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(ui->Hot_Board_label_temp, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_remove_flag(ui->Hot_Board_label_temp, LV_OBJ_FLAG_CHECKABLE);
    lv_label_set_text(ui->Hot_Board_label_temp, "-000.0");
    lv_label_set_long_mode(ui->Hot_Board_label_temp, LV_LABEL_LONG_WRAP);

    //Write style for Hot_Board_label_temp, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Hot_Board_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Hot_Board_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Hot_Board_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Hot_Board_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Hot_Board_label_temp, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Hot_Board_label_temp, &lv_font_montserratMedium_60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Hot_Board_label_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Hot_Board_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Hot_Board_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Hot_Board_label_temp, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Hot_Board_label_temp, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Hot_Board_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Hot_Board_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Hot_Board_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Hot_Board_led_fan
    ui->Hot_Board_led_fan = lv_led_create(ui->Hot_Board);
    lv_obj_set_pos(ui->Hot_Board_led_fan, 24, 139);
    lv_obj_set_size(ui->Hot_Board_led_fan, 5, 5);
    lv_obj_remove_flag(ui->Hot_Board_led_fan, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(ui->Hot_Board_led_fan, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_remove_flag(ui->Hot_Board_led_fan, LV_OBJ_FLAG_CHECKABLE);
    lv_led_set_brightness(ui->Hot_Board_led_fan, 255);
    lv_led_set_color(ui->Hot_Board_led_fan, lv_color_hex(0xff0027));

    //Write codes Hot_Board_led_tr
    ui->Hot_Board_led_tr = lv_led_create(ui->Hot_Board);
    lv_obj_set_pos(ui->Hot_Board_led_tr, 24, 111);
    lv_obj_set_size(ui->Hot_Board_led_tr, 5, 5);
    lv_obj_remove_flag(ui->Hot_Board_led_tr, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(ui->Hot_Board_led_tr, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_remove_flag(ui->Hot_Board_led_tr, LV_OBJ_FLAG_CHECKABLE);
    lv_led_set_brightness(ui->Hot_Board_led_tr, 255);
    lv_led_set_color(ui->Hot_Board_led_tr, lv_color_hex(0xff0027));

    //Write codes Hot_Board_led_sw
    ui->Hot_Board_led_sw = lv_led_create(ui->Hot_Board);
    lv_obj_set_pos(ui->Hot_Board_led_sw, 24, 84);
    lv_obj_set_size(ui->Hot_Board_led_sw, 5, 5);
    lv_obj_remove_flag(ui->Hot_Board_led_sw, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(ui->Hot_Board_led_sw, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_remove_flag(ui->Hot_Board_led_sw, LV_OBJ_FLAG_CHECKABLE);
    lv_led_set_brightness(ui->Hot_Board_led_sw, 255);
    lv_led_set_color(ui->Hot_Board_led_sw, lv_color_hex(0xff0027));

    //Write codes Hot_Board_label_A
    ui->Hot_Board_label_A = lv_label_create(ui->Hot_Board);
    lv_obj_set_pos(ui->Hot_Board_label_A, 284, 4);
    lv_obj_set_size(ui->Hot_Board_label_A, 12, 20);
    lv_obj_remove_flag(ui->Hot_Board_label_A, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(ui->Hot_Board_label_A, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_remove_flag(ui->Hot_Board_label_A, LV_OBJ_FLAG_CHECKABLE);
    lv_label_set_text(ui->Hot_Board_label_A, "A");
    lv_label_set_long_mode(ui->Hot_Board_label_A, LV_LABEL_LONG_WRAP);

    //Write style for Hot_Board_label_A, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Hot_Board_label_A, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Hot_Board_label_A, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Hot_Board_label_A, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Hot_Board_label_A, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Hot_Board_label_A, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Hot_Board_label_A, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Hot_Board_label_A, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Hot_Board_label_A, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Hot_Board_label_A, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Hot_Board_label_A, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Hot_Board_label_A, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Hot_Board_label_A, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Hot_Board_label_A, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Hot_Board_label_A, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Hot_Board_label_CR
    ui->Hot_Board_label_CR = lv_label_create(ui->Hot_Board);
    lv_obj_set_pos(ui->Hot_Board_label_CR, 234, 4);
    lv_obj_set_size(ui->Hot_Board_label_CR, 50, 20);
    lv_obj_remove_flag(ui->Hot_Board_label_CR, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(ui->Hot_Board_label_CR, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_remove_flag(ui->Hot_Board_label_CR, LV_OBJ_FLAG_CHECKABLE);
    lv_label_set_text(ui->Hot_Board_label_CR, "000.0");
    lv_label_set_long_mode(ui->Hot_Board_label_CR, LV_LABEL_LONG_WRAP);

    //Write style for Hot_Board_label_CR, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Hot_Board_label_CR, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Hot_Board_label_CR, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Hot_Board_label_CR, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Hot_Board_label_CR, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Hot_Board_label_CR, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Hot_Board_label_CR, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Hot_Board_label_CR, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Hot_Board_label_CR, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Hot_Board_label_CR, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Hot_Board_label_CR, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Hot_Board_label_CR, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Hot_Board_label_CR, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Hot_Board_label_CR, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Hot_Board_label_CR, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Hot_Board_label_V
    ui->Hot_Board_label_V = lv_label_create(ui->Hot_Board);
    lv_obj_set_pos(ui->Hot_Board_label_V, 194, 4);
    lv_obj_set_size(ui->Hot_Board_label_V, 12, 20);
    lv_obj_remove_flag(ui->Hot_Board_label_V, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(ui->Hot_Board_label_V, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_remove_flag(ui->Hot_Board_label_V, LV_OBJ_FLAG_CHECKABLE);
    lv_label_set_text(ui->Hot_Board_label_V, "V ");
    lv_label_set_long_mode(ui->Hot_Board_label_V, LV_LABEL_LONG_WRAP);

    //Write style for Hot_Board_label_V, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Hot_Board_label_V, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Hot_Board_label_V, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Hot_Board_label_V, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Hot_Board_label_V, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Hot_Board_label_V, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Hot_Board_label_V, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Hot_Board_label_V, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Hot_Board_label_V, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Hot_Board_label_V, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Hot_Board_label_V, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Hot_Board_label_V, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Hot_Board_label_V, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Hot_Board_label_V, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Hot_Board_label_V, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Hot_Board_label_VL
    ui->Hot_Board_label_VL = lv_label_create(ui->Hot_Board);
    lv_obj_set_pos(ui->Hot_Board_label_VL, 144, 4);
    lv_obj_set_size(ui->Hot_Board_label_VL, 50, 20);
    lv_obj_remove_flag(ui->Hot_Board_label_VL, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(ui->Hot_Board_label_VL, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_remove_flag(ui->Hot_Board_label_VL, LV_OBJ_FLAG_CHECKABLE);
    lv_label_set_text(ui->Hot_Board_label_VL, "000.0");
    lv_label_set_long_mode(ui->Hot_Board_label_VL, LV_LABEL_LONG_WRAP);

    //Write style for Hot_Board_label_VL, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Hot_Board_label_VL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Hot_Board_label_VL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Hot_Board_label_VL, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Hot_Board_label_VL, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Hot_Board_label_VL, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Hot_Board_label_VL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Hot_Board_label_VL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Hot_Board_label_VL, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Hot_Board_label_VL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Hot_Board_label_VL, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Hot_Board_label_VL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Hot_Board_label_VL, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Hot_Board_label_VL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Hot_Board_label_VL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Hot_Board_digital_clock_1
    static bool Hot_Board_digital_clock_1_timer_enabled = false;
    ui->Hot_Board_digital_clock_1 = lv_label_create(ui->Hot_Board);
    lv_obj_set_pos(ui->Hot_Board_digital_clock_1, 49, 4);
    lv_obj_set_size(ui->Hot_Board_digital_clock_1, 70, 20);
    lv_obj_remove_flag(ui->Hot_Board_digital_clock_1, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(ui->Hot_Board_digital_clock_1, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_remove_flag(ui->Hot_Board_digital_clock_1, LV_OBJ_FLAG_CHECKABLE);
    lv_label_set_text(ui->Hot_Board_digital_clock_1, "0:00:00");
    if (!Hot_Board_digital_clock_1_timer_enabled) {
        lv_timer_create(Hot_Board_digital_clock_1_timer, 1000, NULL);
        Hot_Board_digital_clock_1_timer_enabled = true;
    }

    //Write style for Hot_Board_digital_clock_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Hot_Board_digital_clock_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Hot_Board_digital_clock_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Hot_Board_digital_clock_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Hot_Board_digital_clock_1, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Hot_Board_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Hot_Board_digital_clock_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Hot_Board_digital_clock_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Hot_Board_digital_clock_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Hot_Board_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Hot_Board_digital_clock_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Hot_Board_digital_clock_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Hot_Board_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Hot_Board_digital_clock_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Hot_Board_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Hot_Board_btn_ext
    ui->Hot_Board_btn_ext = lv_button_create(ui->Hot_Board);
    lv_obj_set_pos(ui->Hot_Board_btn_ext, 14, 4);
    lv_obj_set_size(ui->Hot_Board_btn_ext, 20, 20);
    ui->Hot_Board_btn_ext_label = lv_label_create(ui->Hot_Board_btn_ext);
    lv_label_set_text(ui->Hot_Board_btn_ext_label, "" LV_SYMBOL_LEFT " ");
    lv_label_set_long_mode(ui->Hot_Board_btn_ext_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Hot_Board_btn_ext_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Hot_Board_btn_ext, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Hot_Board_btn_ext_label, LV_PCT(100));

    //Write style for Hot_Board_btn_ext, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Hot_Board_btn_ext, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Hot_Board_btn_ext, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Hot_Board_btn_ext, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Hot_Board_btn_ext, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Hot_Board_btn_ext, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Hot_Board_btn_ext, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Hot_Board_btn_ext, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Hot_Board_btn_ext, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Hot_Board_btn_ext, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Hot_Board_btn_ext, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of Hot_Board.


    //Update current screen layout.
    lv_obj_update_layout(ui->Hot_Board);

    //Init events for screen.
    events_init_Hot_Board(ui);
}

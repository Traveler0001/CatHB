/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_intsup.h>
#include "gui_guider.h"
#include "lvgl.h"
#include "hardware_list.h"
#include "src/widgets/label/lv_label.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

#include "main.h"

extern int start_digital_clock_1_hour_value;
extern int start_digital_clock_1_min_value;
extern int start_digital_clock_1_sec_value;

extern int Hot_Board_digital_clock_1_hour_value;
extern int Hot_Board_digital_clock_1_min_value;
extern int Hot_Board_digital_clock_1_sec_value;

static void start_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        if (start_digital_clock_1_hour_value != Hot_Board_digital_clock_1_hour_value ||
            start_digital_clock_1_min_value != Hot_Board_digital_clock_1_min_value ||
            start_digital_clock_1_sec_value != Hot_Board_digital_clock_1_sec_value)
        {
            start_digital_clock_1_hour_value = Hot_Board_digital_clock_1_hour_value;
            start_digital_clock_1_min_value = Hot_Board_digital_clock_1_min_value;
            start_digital_clock_1_sec_value = Hot_Board_digital_clock_1_sec_value;
        }

        uiIndex = UISTART;
        break;
    }
    default:
        break;
    }
}

static void start_btn_HB_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Hot_Board, guider_ui.Hot_Board_del, &guider_ui.start_del, setup_scr_Hot_Board, LV_SCR_LOAD_ANIM_NONE, 200, 200, true, true);
        break;
    }
    default:
        break;
    }
}

void events_init_start (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->start, start_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->start_btn_HB, start_btn_HB_event_handler, LV_EVENT_ALL, ui);
}

static void Hot_Board_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        uiIndex = UIHOTBOARD;
        if (Hot_Board_digital_clock_1_hour_value != start_digital_clock_1_hour_value ||
            Hot_Board_digital_clock_1_min_value != start_digital_clock_1_min_value ||
            Hot_Board_digital_clock_1_sec_value != start_digital_clock_1_sec_value)
        {
            Hot_Board_digital_clock_1_hour_value = start_digital_clock_1_hour_value;
            Hot_Board_digital_clock_1_min_value = start_digital_clock_1_min_value;
            Hot_Board_digital_clock_1_sec_value = start_digital_clock_1_sec_value;
        }
        app_MAX31865Init();
        break;
    }
    case LV_EVENT_SCREEN_UNLOAD_START:
    {
        uiIndex = UINONE;
        break;
    }
    default:
        break;
    }
}

static void Hot_Board_btn_enter_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
    
        float temp_set = lv_spinbox_get_value(guider_ui.Hot_Board_spinbox_1);
        htraic.tempCtr.temp_set = temp_set;
        char* buf[10];
        snprintf(buf, sizeof(buf), "%3.1f", temp_set/100.0);
        lv_label_set_text(guider_ui.Hot_Board_label_setv_value, buf);
        break;
    }
    default:
        break;
    }
}

static void Hot_Board_spinbox_1_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_KEY:
    {
        break;
    }
    default:
        break;
    }
}

static void Hot_Board_sw_start_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_t * status_obj = lv_event_get_target(e);
        int status = lv_obj_has_state(status_obj, LV_STATE_CHECKED) ? true : false;
        switch (status) {
        case (true):
        {
            htraic.traicStatus.TRAICENSTATUS = true;
            lv_obj_remove_flag(guider_ui.Hot_Board_led_sw, LV_OBJ_FLAG_HIDDEN);
            break;
        }
        case (false):
        {
            htraic.traicStatus.TRAICENSTATUS = false;
            lv_obj_add_flag(guider_ui.Hot_Board_led_sw, LV_OBJ_FLAG_HIDDEN);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void Hot_Board_btn_ext_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        uiIndex = UINONE;
        ui_load_scr_animation(&guider_ui, &guider_ui.start, guider_ui.start_del, &guider_ui.Hot_Board_del, setup_scr_start, LV_SCR_LOAD_ANIM_NONE, 0, 400, true, true);
        break;
    }
    default:
        break;
    }
}

void events_init_Hot_Board (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->Hot_Board, Hot_Board_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Hot_Board_btn_enter, Hot_Board_btn_enter_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Hot_Board_spinbox_1, Hot_Board_spinbox_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Hot_Board_sw_start, Hot_Board_sw_start_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Hot_Board_btn_ext, Hot_Board_btn_ext_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}

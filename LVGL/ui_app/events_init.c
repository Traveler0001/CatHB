/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif


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
    lv_obj_add_event_cb(ui->start_btn_HB, start_btn_HB_event_handler, LV_EVENT_ALL, ui);
}

static void Hot_Board_btn_ext_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.start, guider_ui.start_del, &guider_ui.Hot_Board_del, setup_scr_start, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, true);
        break;
    }
    default:
        break;
    }
}

void events_init_Hot_Board (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->Hot_Board_btn_ext, Hot_Board_btn_ext_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}

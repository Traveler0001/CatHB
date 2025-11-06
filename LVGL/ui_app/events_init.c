/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

// #include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

// #include "gpio.h"
// #include "hard_list.h"

// static void start_event_handler (lv_event_t *e)
// {
//     lv_event_code_t code = lv_event_get_code(e);
//     switch (code) {
//     case LV_EVENT_SCREEN_LOADED:
//     {
//         // 关闭可控硅
//         HAL_GPIO_WritePin(TRAIC_GPIO_Port, TRAIC_Pin, GPIO_PIN_RESET);
//         // 关闭风扇
//         HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, GPIO_PIN_RESET);
//         // 根据FLASH芯片是否存在显示或隐藏"未检测到外部存储器"的提示
//         lv_obj_send_event(guider_ui.start_label_1, LV_EVENT_VALUE_CHANGED, NULL);
//         // 31865失能
//         hmax31865.status_list.enable = MAX31865DISABLE;
//         break;
//     }
//     case LV_EVENT_SCREEN_UNLOAD_START:
//     {

//         break;
//     }
//     default:
//         break;
//     }
// }

// static void start_btn_HB_event_handler (lv_event_t *e)
// {
//     lv_event_code_t code = lv_event_get_code(e);
//     switch (code) {
//     case LV_EVENT_CLICKED:
//     {
//         ui_load_scr_animation(&guider_ui, &guider_ui.Hot_Board, guider_ui.Hot_Board_del, &guider_ui.start_del, setup_scr_Hot_Board, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, false);
//         break;
//     }
//     default:
//         break;
//     }
// }

// static void start_lable_Flash_event_handler (lv_event_t *e)
// {
//     lv_event_code_t code = lv_event_get_code(e);
//     switch (code) {
//     case LV_EVENT_VALUE_CHANGED:
//     {
//         lv_obj_remove_flag(guider_ui.start_lable_Flash, LV_OBJ_FLAG_HIDDEN);
//         lv_obj_add_flag(guider_ui.start_lable_Flash, LV_OBJ_FLAG_HIDDEN);
//         break;
//     }
//     default:
//         break;
//     }
// }

// static void start_digital_clock_1_event_handler (lv_event_t *e)
// {
//     lv_event_code_t code = lv_event_get_code(e);
//     switch (code) {
//     case LV_EVENT_VALUE_CHANGED:
//     {
//         break;
//     }
//     default:
//         break;
//     }
// }

void events_init_start (lv_ui *ui)
{
    // lv_obj_add_event_cb(ui->start, start_event_handler, LV_EVENT_ALL, ui);
    // lv_obj_add_event_cb(ui->start_btn_HB, start_btn_HB_event_handler, LV_EVENT_ALL, ui);
    // lv_obj_add_event_cb(ui->start_lable_Flash, start_lable_Flash_event_handler, LV_EVENT_ALL, ui);
    // lv_obj_add_event_cb(ui->start_digital_clock_1, start_digital_clock_1_event_handler, LV_EVENT_ALL, ui);
}

void events_init(lv_ui *ui)
{
    ;
}

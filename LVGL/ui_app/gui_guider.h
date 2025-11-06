/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"


typedef struct
{
  
	lv_obj_t *start;
	bool start_del;
	lv_obj_t *start_btn_setting;
	lv_obj_t *start_btn_setting_label;
	lv_obj_t *start_btn_usb;
	lv_obj_t *start_btn_usb_label;
	lv_obj_t *start_btn_sd;
	lv_obj_t *start_btn_sd_label;
	lv_obj_t *start_btn_ACMeter;
	lv_obj_t *start_btn_ACMeter_label;
	lv_obj_t *start_btn_HB;
	lv_obj_t *start_btn_HB_label;
	lv_obj_t *start_label_sd;
	lv_obj_t *start_lable_Flash;
	lv_obj_t *start_digital_clock_1;
	// lv_obj_t *Hot_Board;
	// bool Hot_Board_del;
	// lv_obj_t *Hot_Board_temp_set;
	// lv_obj_t *Hot_Board_temp_set_btn_plus;
	// lv_obj_t *Hot_Board_temp_set_btn_minus;
	// lv_obj_t *Hot_Board_btn_enter;
	// lv_obj_t *Hot_Board_btn_enter_label;
	// lv_obj_t *Hot_Board_HB_temp;
	// lv_obj_t *Hot_Board_HB_label_I;
	// lv_obj_t *Hot_Board_BH_value_I;
	// lv_obj_t *Hot_Board_HB_label_U;
	// lv_obj_t *Hot_Board_HB_value_u;
	// lv_obj_t *Hot_Board_HB_label_Fan;
	// lv_obj_t *Hot_Board_HB_label_hoting;
	// lv_obj_t *Hot_Board_led_traic;
	// lv_obj_t *Hot_Board_led_run;
	// lv_obj_t *Hot_Board_digital_clock_1;
	// lv_obj_t *Hot_Board_btn_exitHB;
	// lv_obj_t *Hot_Board_btn_exitHB_label;
	// lv_obj_t *Hot_Board_sw_1;
	lv_obj_t *g_kb_top_layer;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_screen_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, uint32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                  uint32_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                  lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_completed_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_bottom_layer(void);

void setup_ui(lv_ui *ui);

void video_play(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_start(lv_ui *ui);
// void setup_scr_Hot_Board(lv_ui *ui);

LV_FONT_DECLARE(lv_font_montserratMedium_16)
// LV_FONT_DECLARE(lv_font_montserratMedium_12)
// LV_FONT_DECLARE(lv_font_montserratMedium_60)
// LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_18)


#ifdef __cplusplus
}
#endif
#endif

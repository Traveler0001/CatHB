#ifndef __APP_LCD_H
#define __APP_LCD_H

#include "main.h"

#define LVGL 0

#if LVGL
#include "lvgl.h"
#endif

/******* UI *******/
#if LVGL
void app_LcdFill_LVGL(const lv_area_t *area, uint8_t *px_map); // LVGL专用填充函数
void app_LcdDrawPointLVGL(const lv_area_t *area, uint8_t *px_map);
#else
HAL_StatusTypeDef app_LcdShowUI(uint16_t x, uint16_t y, uint16_t color);
HAL_StatusTypeDef app_LcdClear(void);
HAL_StatusTypeDef app_LcdFill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);
HAL_StatusTypeDef app_LcdInit(void);
HAL_StatusTypeDef app_LcdTest(void);

#endif // LVGL

// void lcd_test(void);

#endif
#include "app_lcd.h"
#include "Drivers/drv_lcd.h"
#include "Middlewares/mid_lcd.h"
#include "hardware_list.h"
#include <stdint.h>

#if 1
// 显示UI（预留，未实现）
HAL_StatusTypeDef app_LcdShowUI(uint16_t x, uint16_t y, uint16_t color)
{
    return  HAL_OK;
}

// 清屏
HAL_StatusTypeDef app_LcdClear(void)
{
    uint16_t xsta = 0, ysta = 0;
    return mid_LcdFill(&xsta, &ysta, &lcd.lcdInfo.lcdW, &lcd.lcdInfo.lcdH, &lcd.lcdInfo.clearColor);
}

// ָ填充
HAL_StatusTypeDef app_LcdFill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    return mid_LcdFill(&xsta, &ysta, &xend, &yend, &color);
}  

HAL_StatusTypeDef app_Init(void)
{
    lcd.lcdInfo.lcdH = 240;
    lcd.lcdInfo.lcdW = 320;
    lcd.lcdInfo.clearColor = BLACK;
    lcd.lcdMode.dmaMode = 0;
    lcd.lcdMode.spiMode = 1;
    return drv_LcdInit();
}

HAL_StatusTypeDef app_LcdTest(void)
{
    return mid_LcdTest();
}

#endif
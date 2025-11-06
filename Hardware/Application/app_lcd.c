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
    #if LCDLVGL
        return  HAL_ERROR;
    #else
        uint16_t xsta = 0, ysta = 0;
        return mid_LcdFill(&xsta, &ysta, &lcd.lcdInfo.lcdW, &lcd.lcdInfo.lcdH, &lcd.lcdInfo.clearColor);
    #endif // LCDLVGL
}

// ָ填充
HAL_StatusTypeDef app_LcdFill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    #if LCDLVGL
        return  HAL_ERROR;
    #else
    return mid_LcdFill(&xsta, &ysta, &xend, &yend, &color);
    #endif // LCDLVGL
}  

HAL_StatusTypeDef app_LcdInit(void)
{
    lcd.lcdInfo.lcdH = 240;
    lcd.lcdInfo.lcdW = 320;
    lcd.lcdInfo.clearColor = BLACK;
    lcd.lcdMode.dmaMode = 1;
    lcd.lcdMode.spiMode = 1;
    return drv_LcdInit();
}

HAL_StatusTypeDef app_LcdTest(void)
{
    #if LCDTEST
        ;return mid_LcdTest();
    #else
        return  HAL_ERROR;
    #endif // LCDTEST
}

#endif
#ifndef __DRV_LCD_H
#define __DRV_LCD_H


#include "main.h"
#include <stdint.h>
// #include "stm32f4xx_hal_def.h"

#define USE_HORIZONTAL 3  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

typedef struct{
    uint16_t lcdW;          // LCD显示屏宽度(像素)
    uint16_t lcdH;          // LCD显示屏高度(像素)
    uint16_t clearColor;    // 清屏默认颜色
} LCD_INFO;

typedef struct{
    uint8_t spiMode;  // SPI模式 0:软件SPI 1:硬件SPI
    uint8_t dmaMode;  // DMA模式 0:不使用DMA 1:使用DMA
}LCD_MODE;

typedef struct{
    LCD_INFO lcdInfo;   // LCD显示屏相关信息设置
    LCD_MODE lcdMode;   // LCD spi模式相关设置
} LCD_Handle;

extern  LCD_Handle lcd; // LCD操作句柄

// void LCD_GPIO_Init(void);//初始化GPIO
HAL_StatusTypeDef drv_LcdWRData8(uint8_t dat);//写入一个字节
HAL_StatusTypeDef drv_LcdWRData16(uint16_t dat);//写入两个字节
HAL_StatusTypeDef drv_LcdWRDatas(uint8_t *dat, uint16_t len);
HAL_StatusTypeDef drv_LcdWRReg(uint8_t dat);//写入一个指令
HAL_StatusTypeDef drv_LcdAddressSet(uint16_t *x1, uint16_t *y1, uint16_t *x2, uint16_t *y2);//设置坐标函数
HAL_StatusTypeDef drv_LcdInit(void);//LCD初始化

#endif
#ifndef __TFT_INIT_H
#define __TFT_INIT_H

#include "gpio.h"
#include "main.h"

#define HARDWARE_SPI 1   // 使用硬件SPI
#define USE_HORIZONTAL 3  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 320
#else
#define LCD_W 320
#define LCD_H 240
#endif

//-----------------LCD端口定义----------------
#define LCD_RES_Clr() HAL_GPIO_WritePin(TFT_RES_GPIO_Port, TFT_RES_Pin, GPIO_PIN_RESET) // RES -->0
#define LCD_RES_Set() HAL_GPIO_WritePin(TFT_RES_GPIO_Port, TFT_RES_Pin, GPIO_PIN_SET)   // RES -->1

#define LCD_DC_Clr() HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_RESET) // DC -->0
#define LCD_DC_Set() HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET)   // DC -->1

#define LCD_CS_Clr() HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET) // CS -->0
#define LCD_CS_Set() HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET)   // CS -->1

#define LCD_BLK_Clr() HAL_GPIO_WritePin(TFT_BK_GPIO_Port, TFT_BK_Pin, GPIO_PIN_RESET) // BLK -->0
#define LCD_BLK_Set() HAL_GPIO_WritePin(TFT_BK_GPIO_Port, TFT_BK_Pin, GPIO_PIN_SET)   // BLK -->1                                                  // LCD初始化

// void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(uint8_t dat);//模拟SPI时序
void LCD_WR_DATA8(uint8_t dat);//写入一个字节
void LCD_WR_DATA(uint16_t dat);//写入两个字节
void LCD_WR_DATAS(uint8_t *dat, uint16_t len);
void LCD_WR_REG(uint8_t dat);//写入一个指令
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);//设置坐标函数
void LCD_Init(void);//LCD初始化

#endif
#ifndef __TFT_LCD_H
#define __TFT_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

#define HLW8032 0
#define MAX31865 1
#define W25QXX 0
#define TRAIC 1

#define LCD 1
#define ST7789 1

#if HLW8032
#include "hlw8032.h"
#endif

#if MAX31865
#include "./Drivers/drv_max31865.h"
#include "./Middlewares/mid_max31865.h"
#include "./Application/app_max31865.h"
#endif

#if W25QXX
#include "w25qxx.h"
#endif

#if LCD
    #ifdef ST7789
    #include "./Drivers/st7789.h"
    #endif // ST7789
#include "./Drivers/drv_lcd.h"
#include "./Middlewares/mid_lcd.h"
#include "./Application/app_lcd.h"
#endif

#if TRAIC
    #include "./Drivers/drv_traic.h"
#endif // TRAIC

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
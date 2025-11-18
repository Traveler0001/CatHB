#ifndef __DRV_MAX31865_H
#define __DRV_MAX31865_H

#include "main.h"
#include "gpio.h"

//*
//* 寄存器地址    寄存器名称            读写属性    位编号      位名称 / 功能                   详细说明
//*   00h         config              R/W         D7        BIAS（偏置电压控制）	    0：禁用 VBIAS（低功耗模式）；1：启用 VBIAS 
//*                                               D6        转换模式	               0：单次转换模式（需通过D5触发）；1：自动连续转换模式 
//*                                               D5	      1-Shot（单次转换触发）      1：在单次模式下启动一次转换（转换后自动清零） 
//*                                               D4	      3-Wire（RTD 接线模式）    0：2 线或 4 线模式；1：3 线模式（用于消除引线电阻影响） 
//*                                               D3-D2	    故障检测周期               00：无自动检测；01：自动检测（每转换一次）；10：每 4 次转换检测一次；11：每8次转换检测一次 
//*                                               D1	      故障状态清零                1：清除故障寄存器（07h）的所有故障标志（需同时将 D5、D3、D2 置 0） 
//*                                               D0        50/60Hz滤波选择	          0：60Hz 滤波（适用于美洲电网）；1：50Hz 滤波（适用于欧洲 /亚洲电网） 
//*   01h	        RTD MSB             R	          D7-D0     RTD电阻数据高 8 位	        存储 RTD 电阻测量值的高 8 位（与 02h 的 D7-D1 组成 15位有效数据） 
//*   02h	        RTD LSB	            R	          D7-D1     RTD 电阻数据低7 位	        存储 RTD 电阻测量值的低 7 位（与 01h 共同组成 15位数据，分辨率为 R_REF/32768） 
//*                                               D0	      FAULT（故障标志）           0：无故障；1：检测到 RTD 相关故障（需结合 07h 寄存器查看详情） 
//*   03h         HIGHTHRESHOLD MSB 	R/W	        D7-D0     高故障门限高 8 位           存储 RTD电阻上限阈值的高 8 位（用于故障检测） 
//*   04h	        HIGHTHRESHOLD LSB   R/W	        D7-D0     高故障门限低 8 位	        存储 RTD 电阻上限阈值的低 8位（仅高 7 位有效，D0 无意义） 
//*   05h	        LOWTHRESHOLD MSB    R/W         D7-D0     低故障门限高 8 位	        存储 RTD 电阻下限阈值的高 8位（用于故障检测） 
//*   06h	        LOWTHRESHOLD LSB    R/W	        D7-D0     低故障门限低 8 位	        存储 RTD 电阻下限阈值的低 8 位（仅高 7 位有效，
//*                                               D0        无意义
//*   07h	        FAULT_STATUS        R	          D7	      高温故障（RTD> 高门限）	    1：RTD 电阻值超过高故障门限 
//*                                               D6	      低温故障（RTD < 低门限）    1：RTD 电阻值低于低故障门限 
//*                                               D5	      RTD 输入开路故障	          1：检测到 RTD开路 
//*                                               D4	      RTD 引线短路到 VCC	       1：检测到 RTD 引线短路到电源电压 
//*                                               D3        RTD 引线短路到 GND	       1：检测到 RTD 引线短路到地 
//*                                               D2	      过压 / 欠压故障            1：检测到 VBIAS 过压或欠压 
//*                                               D1-D0	    保留位 未使用，读取时恒为 0
//*/

// register addresses
#define MAX31865_REG_CONFIG 0x00       // 配置寄存器地址
#define MAX31865_REG_RTD_MSB 0x01      // RTD数据寄存器（MSB）
#define MAX31865_REG_RTD_LSB 0x02      // RTD数据寄存器（LSB）
#define MAX31865_REG_HFAULT_MSB 0x03   // 高阈值寄存器（MSB）
#define MAX31865_REG_HFAULT_LSB 0x04   // 高阈值寄存器（LSB）
#define MAX31865_REG_LFAULT_MSB 0x05   // 低阈值寄存器（MSB）
#define MAX31865_REG_LFAULT_LSB 0x06   // 低阈值寄存器（LSB）
#define MAX31865_REG_FAULT_STATUS 0x07 // 故障状态寄存器地址

// configuration bits
#define MAX31865_CONFIG_VBIAS_ON 1  // 0x00[7:1]: 启用 VBIAS
#define MAX31865_CONFIG_VBIAS_OFF 0 // 0x00[7:1]: 禁用 VBIAS（低功耗模式）
#define MAX31865_CONFIG_MODE_AUTO 1 // 0x00[6:1]: 自动连续转换模式
#define MAX31865_CONFIG_MODE_ONESHOT                                           \
  1 // 0x00[6:1]: 单次转换模式（需通过 D5 触发）
#define MAX31865_CONFIG_1SHOT                                                  \
  1 // 0x00[5:1]: 单次模式下启动一次转换（转换后自动清零）
#define MAX31865_CONFIG_3WIRE 1 // 0x00[4:1]: 3 线模式（用于消除引线电阻影响）
#define MAX31865_CONFIG_2_4_WIRE 0    // 0x00[4:1]: 2 线或 4 线模式
#define MAX31865_CONFIG_FDC_DISABLE 0 // 0x00[3:2]: 无自动检测
#define MAX31865_CONFIG_FDC_1SHOT 1   // 0x00[3:2]: 每转换一次自动检测故障
#define MAX31865_CONFIG_FDC_4SHOT 2   // 0x00[3:2]: 每 4 次转换检测一次
#define MAX31865_CONFIG_FDC_8SHOT 3   // 0x00[3:2]: 每 8 次转换检测一次
#define MAX31865_CONFIG_FAULT_DETECT                                           \
  1 // 0x00[1:1]: 清除故障寄存器（07h）的所有故障标志（需同时将 D5、D3、D2 置
    // 0）
#define MAX31865_CONFIG_FAULT_DETECT_DISABLE 0 // 0x00[1:1]: 不清除故障寄存器
#define MAX31865_CONFIG_50HZ 1 // 0x00[0:1]: 50Hz 滤波（适用于欧洲 / 亚洲电网）
#define MAX31865_CONFIG_60HZ 0 // 0x00[0:1]: 60Hz 滤波（适用于美洲电网）
#define MAX31865_CONFIG_NONE 0 // 0x00[7:0]: 无配置（默认值）

// fault status bits
#define MAX31865_FAULT_HIGHTHRESHOLD 1    // 0x07[7:1]: 高阈值故障
#define MAX31865_FAULT_LOWTHRESHOLD 1     // 0x07[6:1]: 低阈值故障
#define MAX31865_FAULT_RTD_SHORT 1        // 0x07[5:1]: RTD 开路故障
#define MAX31865_FAULT_RTD_SHORT_VCC 1    // 0x07[4:1]: RTD 短路到 VCC 故障
#define MAX31865_FAULT_RTD_SHORT_GND 1    // 0x07[3:1]: RTD 短路到 GND 故障
#define MAX31865_FAULT_VBIAS_OVER_UNDER 1 // 0x07[2:1]: VBIAS 过压或欠压故障
#define MAX31865_FAULT_FDL_OPEN 1         // 0x07[4:1]: FDL 开路故障
#define MAX31865_FAULT_NONE 0             // 0x07[1:2]: 保留位
#define MAX31865_FAULT_RTD (1 << 1)       // RTD故障标志位

#define MAX31865_MODE_24W  HAL_GPIO_WritePin(PT100_MODE_GPIO_Port, PT100_MODE_Pin, GPIO_PIN_SET)
#define MAX31865_MODE_3W  HAL_GPIO_WritePin(PT100_MODE_GPIO_Port, PT100_MODE_Pin, GPIO_PIN_RESET)
#define MAX31865_RDY HAL_GPIO_ReadPin(MAX31865_DRDY_GPIO_Port, MAX31865_DRDY_Pin)

#define max31865statusInfoMax 30

typedef enum {
  MAX31865OK,                // MAX31865正常
  MAX31865NOTRADY,           // MAX31865数据未准备完成
  MAX31865SPIERR,            // SPI总线错误
  MAX31865OVERHIGHTHRESHOLD, // 高温故障（RTD > 高门限
  MAX31865LOWLOWTHRESHOLD,   // 低温故障（RTD < 低门限
  MAX31865RTDOPEN,           // RTD 输入开路故障
  MAX31865RTDSHORTVCC,       // RTD 引线短路到 VCC
  MAX31865RTDSHORTGND,       // RTD 引线短路到 GND
  MAX31865VBIASOVERORUNDER,  // VBIAS 过压 / 欠压故障
  MAX31865NULL               // 初始化状态
} MAX31685_STATUS;

typedef struct {
  uint8_t bias : 1;        // BIAS（偏置电压控制）
  uint8_t mode : 1;        // 转换模式
  uint8_t oneShot : 1;     // 1-Shot（单次转换触发）
  uint8_t wireMode : 1;    // 3-Wire（RTD 接线模式）
  uint8_t fdcMode : 2;     // 故障检测周期
  uint8_t faultDetect : 1; // 故障状态清零
  uint8_t frequency : 1;   // // 50/60Hz 滤波选择
  uint8_t config;          // 整个配置寄存器的值
} MAX31865_Config;

typedef struct {
  uint8_t highThreshold : 1;    // 高温故障（RTD > 高门限）
  uint8_t lowThreshold : 1;     // 低温故障（RTD < 低门限）
  uint8_t rtdOpen : 1;          // RTD 输入开路故障
  uint8_t rtdShortToVCC : 1;    // RTD 引线短路到 VCC
  uint8_t rtdShortToGND : 1;    // RTD 引线短路到 GND
  uint8_t vBiasOverOrUnder : 1; // VBIAS 过压 / 欠压故障
  uint8_t none : 2;             // 保留位，通常为 0
  uint8_t status;               // 故障状态寄存器的值
} MAX31865_Status_List;

typedef struct {
  float temperature; // Temperature in Celsius
  uint8_t fault;     // 故障标志
  uint16_t rawData;  // 原始16位数据
} MAX31865_RtdData;

typedef struct {
  MAX31865_Config max_config;           // MAX31865配置寄存器
  MAX31865_Status_List max_status_list; // MAX31865故障状态寄存器
  MAX31865_RtdData max_rtdData;         // MAX31865 RTD数据结构体
  MAX31685_STATUS max_status;
} MAX31865_HandleTypeDef;

extern uint8_t max31865statusInfo[max31865statusInfoMax];
extern MAX31865_HandleTypeDef hmax31865;

MAX31685_STATUS drv_MAX31865WriteRegister(uint8_t reg, uint8_t value);
MAX31685_STATUS drv_MAX31865ReadRegister(uint8_t reg, uint8_t *rx_buf);

#endif // !__DRV_MAX31865_H

#include "hardware_list.h"
#include "usart.h"
#include "stdio.h"
#include <math.h>

#if 1

#define WIRE_2_4
#define showOfUsart 0

/**
 * @brief Initialize the MAX31865 sensor,Set the configuration register to default values
 *
 * @return uint8_t
 */
MAX31685_STATUS app_MAX31865Init(void)
{
    hmax31865.max_config.bias = MAX31865_CONFIG_VBIAS_ON;
    hmax31865.max_config.mode = MAX31865_CONFIG_MODE_AUTO;
    hmax31865.max_config.oneShot = MAX31865_CONFIG_NONE;
    hmax31865.max_config.wireMode = MAX31865_CONFIG_2_4_WIRE;
    hmax31865.max_config.fdcMode = MAX31865_CONFIG_FDC_DISABLE;
    hmax31865.max_config.faultDetect = MAX31865_CONFIG_FAULT_DETECT;
    hmax31865.max_config.frequency = MAX31865_CONFIG_50HZ;

    hmax31865.max_config.config =   hmax31865.max_config.bias << 7 | hmax31865.max_config.mode << 6 | hmax31865.max_config.oneShot << 5 |
                                    hmax31865.max_config.wireMode << 4 | hmax31865.max_config.fdcMode << 2 | hmax31865.max_config.faultDetect << 1 |
                                    hmax31865.max_config.frequency;
    return drv_MAX31865WriteRegister(MAX31865_REG_CONFIG, hmax31865.max_config.config);
}


MAX31685_STATUS app_MAX31865Read()
{
    if (MAX31865_RDY == GPIO_PIN_SET) {
        hmax31865.max_status = MAX31865NOTRADY;
        return MAX31865NOTRADY;    
    }

    hmax31865.max_status = mid_Max31865RDataProcess();

    switch (hmax31865.max_status)
    {
    // case MAX31865OK:
    //     snprintf((char *)max31865statusInfo, max31865statusInfoMax - 2, "DATA OK");
    //     break;
    // case MAX31865NOTRADY:
    //     snprintf((char *)max31865statusInfo, max31865statusInfoMax - 2, "DATA NOT READY");
    //     break;
    // case MAX31865SPIERR:
    //     snprintf((char *)max31865statusInfo, max31865statusInfoMax - 2, "SPI BUS REEOR");
    //     break;
    // case MAX31865OVERHIGHTHRESHOLD:
    //     snprintf((char *)max31865statusInfo, max31865statusInfoMax - 2, "OVER OF HIGHTHRESHOLD");
    //     break;
    // case MAX31865LOWLOWTHRESHOLD:
    //     snprintf((char *)max31865statusInfo, max31865statusInfoMax - 2, "LOW OF LOWTHRESHOLD");
    //     break;
    // case MAX31865RTDOPEN:
    //     snprintf((char *)max31865statusInfo, max31865statusInfoMax - 2, "RTD OPEN");
    //     break;
    // case MAX31865RTDSHORTVCC:
    //     snprintf((char *)max31865statusInfo, max31865statusInfoMax - 2, "RTD SHORT TO VCC");
    //     break;
    // case MAX31865RTDSHORTGND:
    //     snprintf((char *)max31865statusInfo, max31865statusInfoMax - 2, "RTD SHORT TO GNDK");
    //     break;
    // case MAX31865VBIASOVERORUNDER:
    //     snprintf((char *)max31865statusInfo, max31865statusInfoMax - 2, "VBIAS OVER OR UNDER");
    //     break;
    }

#if showOfUsart
    HAL_UART_Transmit(&huart1, statusInfoMax, sizeof(statusInfoMax), HAL_MAX_DELAY);
#endif
    return hmax31865.max_status;
}

/**
 * @brief 读取MAX31865配置寄存器
 *
 * 该函数从MAX31865的配置寄存器中读取当前配置，并将其解析为结构体成员变量。
 */
// uint8_t MAX31865_ReadConfig(void)
// {
//     uint8_t config = 0;
//     MAX31865_ReadRegister(MAX31865_REG_CONFIG, &config);

//     max_config.bias = (config >> 7) & 0x01;
//     max_config.mode = (config >> 6) & 0x01;
//     max_config.oneShot = (config >> 5) & 0x01;
//     max_config.wireMode = (config >> 4) & 0x01; // 1 bits for wire mode
//     max_config.fdcMode = (config >> 2) & 0x03;  // 2 bits for FDC mode
//     max_config.faultDetect = (config >> 1) & 0x01;
//     max_config.frequency = config & 0x01; // 1 bit for frequency
//     max_config.config = config;           // Store the full configuration value

//     return max_config.config; // 返回当前配置寄存器的值
// }

// void MAX31865_Test(void)
// {
//     if (MAX31685_RDY == 0)
//     {
//         if (MAX31685_RDY == GPIO_PIN_SET)
//         {
//             uint8_t Fault_Status = 0;                                            // Fault status variable
//             uint8_t strTemp[50] = {0};                                           // Buffer for temperature string
//             Fault_Status = MAX31865_ReadFaultStatus();                           // Get Fault_Status
//             HAL_UART_Transmit(&huart1, strTemp, sizeof(strTemp), HAL_MAX_DELAY); //
//             sprintf((char *)strTemp, "tempture: %f C\r\n Fault_Status: %d\r\n", max_rtdData.temperature, Fault_Status);
//             HAL_UART_Transmit(&huart1, strTemp, sizeof(strTemp), HAL_MAX_DELAY);
//             HAL_UART_Transmit(&huart1, "Fault_Status:", 14, HAL_MAX_DELAY);
//             HAL_UART_Transmit(&huart1, &Fault_Status, 1, HAL_MAX_DELAY);
//         }
//     }
//     MAX31865_Init();
//     HAL_Delay(200);
// }

#endif // 0
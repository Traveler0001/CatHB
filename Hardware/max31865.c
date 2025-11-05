#include "max31865.h"
#include "usart.h"
#include "spi.h"
#include "gpio.h"
// #include "FreeRTOS.h"
// #include "task.h"
#include "stdio.h"
#include <math.h>

#if 0



#define WIRE_2_4
#define FREERTOS 1
#define showOfUsart 0
MAX31865_Config max_config;           // Initialize the config structure
MAX31865_Status_List max_status_list; // Initialize the status structure
MAX31865_RtdData max_rtdData;         // Initialize the RTD data structure
MAX31685_STATUS max_status;

uint8_t max31865statusInfo[max31865statusInfoMax];

/**
 * @brief SPI写入寄存器函数
 *
 * @param reg 寄存器地址
 * @param value 寄存器值
 * @return uint8_t  SPI发送状态, 返回0表示成功，1表示失败
 */
MAX31685_STATUS MAX31865_WriteRegister(uint8_t reg, uint8_t value)
{
    uint8_t spi_state;

    // Write a value to the specified register
    reg |= 0x80; // Clear the MSB to indicate a write operation

    uint8_t data[2] = {reg, value};

    max31865_cs_busy;
    __nop();
    spi_state = HAL_SPI_Transmit(&hspi2, data, 2, 100);
    max31865_cs_idle;

    if (spi_state != HAL_OK)
        return MAX31865SPIERR;

    return MAX31865OK; // Return 0 if there wasn`t an error
}

/**
 * @brief spi读取寄存器函数
 *
 * @param reg 寄存器值
 * @return uint8_t SPI接收数据
 */
MAX31685_STATUS MAX31865_ReadRegister(uint8_t reg, uint8_t *rx_buf)
{
    uint8_t spi_state;

    reg &= 0x7f; // read mode

    max31865_cs_busy;
    HAL_SPI_Transmit(&hspi2, &reg, 1, HAL_MAX_DELAY); // Send the register address
    spi_state = HAL_SPI_Receive(&hspi2, rx_buf, 1, 500);
    max31865_cs_idle;

    if (spi_state == HAL_OK)
        return MAX31865OK;
    else
        return MAX31865SPIERR; // Check the SPI state and handle errors
}

/**
 * @brief Initialize the MAX31865 sensor,Set the configuration register to default values
 *
 * @return uint8_t
 */
MAX31685_STATUS MAX31865_Init(void)
{
    MAX31685_STATUS status;
    max_config.bias = MAX31865_CONFIG_VBIAS_ON;
    max_config.mode = MAX31865_CONFIG_MODE_AUTO;
    max_config.oneShot = MAX31865_CONFIG_NONE;
    max_config.wireMode = MAX31865_CONFIG_2_4_WIRE;
    max_config.fdcMode = MAX31865_CONFIG_FDC_DISABLE;
    max_config.faultDetect = MAX31865_CONFIG_FAULT_DETECT;
    max_config.frequency = MAX31865_CONFIG_60HZ;

    max_config.config = max_config.bias << 7 | max_config.mode << 6 | max_config.oneShot << 5 |
                        max_config.wireMode << 4 | max_config.fdcMode << 2 | max_config.faultDetect << 1 |
                        max_config.frequency;
    // return (MAX31865_WriteRegister(MAX31865_REG_CONFIG, max_config.config) == HAL_OK) ? MAX31865OK : MAX31865SPIERR;
    uint8_t data[2] = {MAX31865_REG_CONFIG, max_config.config};
    max31865_cs_busy;
    status = (HAL_SPI_Transmit(&hspi2, data, 2, 100) == HAL_OK) ? MAX31865OK : MAX31865SPIERR;
    max31865_cs_idle;
    return status;
}

// 定义PT100的系数(IEC 751标准)
#define A 3.9083e-3f
#define B -5.775e-7f
#define C -4.183e-12f
#define R0 100.0f    // PT100在0℃时的电阻值
#define R_REF 430.0f // 参考电阻值(根据硬件配置修改)

// 计算温度值
static float calculateTemperature(float rt)
{
    float r = rt / R0; // 电阻比

    if (r >= 1.0f)
    {
        // 0℃ ~ 850℃范围，解二次方程
        float discriminant = A * A - 4 * B * (1 - r);
        if (discriminant < 0)
            return NULL; // 无效值
        return (-A + sqrtf(discriminant)) / (2 * B);
    }
    else
    {
        // -200℃ ~ 0℃范围，使用三次方程迭代求解
        float t = -242.02f + 2.2228f * rt; // 初始近似值
        for (int i = 0; i < 5; i++)
        { // 迭代5次提高精度
            float t3 = t * t * t;
            float f = R0 * (1 + A * t + B * t * t + C * (t - 100) * t3) - rt;
            float df = R0 * (A + 2 * B * t + C * (4 * t3 - 300 * t * t + 2 * t * 100));
            t -= f / df; // 牛顿-拉夫逊迭代
        }
        return t;
    }
}

/**
 * @brief 读取MAX31865的RTD值
 *
 * 该函数从MAX31865的RTD寄存器中读取温度数据，并将其转换为摄氏度。
 * 如果DRDY引脚为低电平，表示数据未准备好，直接返回1。
 * 如果读取到的RTD数据有故障，则设置故障标志并返回1。
 * 否则，计算温度并返回0表示成功。
 *
 * @return uint8_t 返回0表示成功，1表示故障或数据未准备好
 */
static MAX31685_STATUS MAX31865_ReadRTDData(void)
{
    uint8_t rtd_msb, rtd_lsb;

    // 初始化数据结构体
    max_rtdData.temperature = 0;
    max_rtdData.fault = 0;
    max_rtdData.rawData = 0;

    if (MAX31865_RDY == GPIO_PIN_SET)
        return MAX31865NOTRADY; // 如果DRDY引脚为高电平，表示数据未准备好，直接返回1

    // 正确拼接16位温度数据
    if (MAX31865_ReadRegister(MAX31865_REG_RTD_MSB, &rtd_msb) != HAL_OK)
        return MAX31865SPIERR;
    if (MAX31865_ReadRegister(MAX31865_REG_RTD_LSB, &rtd_lsb) != HAL_OK)
        return MAX31865SPIERR;

    // 组合15位有效数据(忽略LSB的D0位)
    max_rtdData.rawData = ((rtd_msb << 8) | rtd_lsb) >> 1;

    // 计算实际电阻值
    max_rtdData.temperature = (float)max_rtdData.rawData * (R_REF / 32768.0f);

    // 计算温度值（分辨率0.0078125℃）
    max_rtdData.temperature = calculateTemperature(max_rtdData.temperature);

    return MAX31865OK; // 返回0表示读取成功
}

/**
 * @brief 读取MAX31865故障状态寄存器
 *
 * 该函数从MAX31865的故障状态寄存器中读取当前故障状态，并将其解析为结构体成员变量。
 */
static MAX31685_STATUS MAX31865_ReadFaultStatus(void)
{
    // 读取故障状态寄存器
    if (MAX31865_ReadRegister(MAX31865_REG_FAULT_STATUS, &max_status_list.status) != HAL_OK)
        return MAX31865SPIERR;

    // max_status_list.status = 0;

    // 解析故障状态
    max_status_list.highThreshold = (max_status_list.status >> 7) & 0x01;    // 高阈值故障
    max_status_list.lowThreshold = (max_status_list.status >> 6) & 0x01;     // 低阈值故障
    max_status_list.rtdOpen = (max_status_list.status >> 5) & 0x01;          // RTD开路故障
    max_status_list.rtdShortToVCC = (max_status_list.status >> 4) & 0x01;    // RTD短路到VCC故障
    max_status_list.rtdShortToGND = (max_status_list.status >> 3) & 0x01;    // RTD短路到GND故障
    max_status_list.vBiasOverOrUnder = (max_status_list.status >> 2) & 0x01; // VBIAS过压或欠压故障
    max_status_list.none = (max_status_list.status & 0x03);                  // 保留位，通常为0

    return MAX31865OK;
}

MAX31685_STATUS hlw8032_RDataProcess()
{
    max_status = MAX31865_ReadFaultStatus();

    if (max_status == MAX31865SPIERR)
        max_status = MAX31865SPIERR;

    if (max_status_list.highThreshold == MAX31865_FAULT_HIGHTHRESHOLD)
        max_status = MAX31865OVERHIGHTHRESHOLD; // MAX31865 OVER OF HIGHTHRESHOLD
    else if (max_status_list.lowThreshold == MAX31865_FAULT_HIGHTHRESHOLD)
        max_status = MAX31865LOWLOWTHRESHOLD; // MAX31865 LOW OF LOWTHRESHOLD
    else if (max_status_list.rtdOpen == MAX31865_FAULT_RTD_SHORT)
        max_status = MAX31865RTDOPEN; // MAX31865 RTD OPEN
    else if (max_status_list.rtdShortToVCC == MAX31865_FAULT_RTD_SHORT_VCC)
        max_status = MAX31865RTDSHORTVCC; // MAX31865 RTD SHORT TO VCC
    else if (max_status_list.rtdShortToGND == MAX31865_FAULT_RTD_SHORT_GND)
        max_status = MAX31865RTDSHORTGND; // MAX31865 RTD SHORT TO GND
    else if (max_status_list.vBiasOverOrUnder == MAX31865_FAULT_VBIAS_OVER_UNDER)
        max_status = MAX31865VBIASOVERORUNDER; // MAX31865 VBIAS OVER OR UNDER
    max_status = MAX31865_ReadRTDData();
    if (max_status == 0)
        max_status = MAX31865OK;
    else
        MAX31865_Init();

    return max_status;
}

MAX31685_STATUS MAX31865_Read()
{
    max_status = hlw8032_RDataProcess();

    switch (max_status)
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
    return max_status;
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
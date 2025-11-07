#include "hardware_list.h"
#include "spi.h"
#include "gpio.h"

#if 1

#define max31865_cs_idle HAL_GPIO_WritePin(MAX31865_CS_GPIO_Port, MAX31865_CS_Pin, GPIO_PIN_SET)   // Set CS pin high (idle state)
#define max31865_cs_busy HAL_GPIO_WritePin(MAX31865_CS_GPIO_Port, MAX31865_CS_Pin, GPIO_PIN_RESET) // Set CS pin low (busy state)

MAX31865_HandleTypeDef hmax31865;

uint8_t max31865statusInfo[max31865statusInfoMax];

/**
 * @brief SPI写入寄存器函数
 *
 * @param reg 寄存器地址
 * @param value 寄存器值
 * @return uint8_t  SPI发送状态, 返回0表示成功，1表示失败
 */
MAX31685_STATUS drv_MAX31865WriteRegister(uint8_t reg, uint8_t value)
{
    // Write a value to the specified register
    reg |= 0x80; // Clear the MSB to indicate a write operation

    uint8_t data[2] = {reg, value};

    max31865_cs_busy;
    // __nop();
    HAL_SPI_Transmit_DMA(&hspi2, data, 2);
    // 等待DMA传输完成
    while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY) ;

    max31865_cs_idle;

    return MAX31865OK; // Return 0 if there wasn`t an error
}

/**
 * @brief spi读取寄存器函数
 *
 * @param reg 寄存器值
 * @return uint8_t SPI接收数据
 */
MAX31685_STATUS drv_MAX31865ReadRegister(uint8_t reg, uint8_t *rx_buf)
{
    reg &= 0x7f; // read mode

    max31865_cs_busy;
    HAL_SPI_Transmit_DMA(&hspi2, &reg, 1); // Send the register address
    HAL_SPI_Receive_DMA(&hspi2, rx_buf, 1);
    // 等待DMA传输完成
    while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY) ;
    max31865_cs_idle;

    // if (spi_state == HAL_OK)
        return MAX31865OK;
    // else
    //     return MAX31865SPIERR; // Check the SPI state and handle errors
}

#endif // 0
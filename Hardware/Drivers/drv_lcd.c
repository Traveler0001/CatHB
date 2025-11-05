#include "drv_lcd.h"
#include "st7789.h"
#include "gpio.h"
#include "spi.h"
// #include "stm32f4xx_hal_def.h"

#if 1

LCD_Handle lcd;

//-----------------LCD端口定义----------------
// #define LCD_BLK_Clr() HAL_GPIO_WritePin(TFT_BK_GPIO_Port, TFT_BK_Pin, GPIO_PIN_RESET) // BLK -->0
// #define LCD_BLK_Set() HAL_GPIO_WritePin(TFT_BK_GPIO_Port, TFT_BK_Pin, GPIO_PIN_SET)   // BLK -->1 

// #define LCD_RES_Clr() HAL_GPIO_WritePin(TFT_RES_GPIO_Port, TFT_RES_Pin, GPIO_PIN_RESET) // RES -->0
// #define LCD_RES_Set() HAL_GPIO_WritePin(TFT_RES_GPIO_Port, TFT_RES_Pin, GPIO_PIN_SET)   // RES -->1
#define LCD_RES_Clr() HAL_GPIO_WritePin(TFT_BK_GPIO_Port, TFT_BK_Pin, GPIO_PIN_RESET) // BLK -->0
#define LCD_RES_Set() HAL_GPIO_WritePin(TFT_BK_GPIO_Port, TFT_BK_Pin, GPIO_PIN_SET)   // BLK -->1 

#define LCD_BLK_Clr() HAL_GPIO_WritePin(TFT_RES_GPIO_Port, TFT_RES_Pin, GPIO_PIN_RESET) // RES -->0
#define LCD_BLK_Set() HAL_GPIO_WritePin(TFT_RES_GPIO_Port, TFT_RES_Pin, GPIO_PIN_SET)   // RES -->1

#define LCD_DC_Clr() HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_RESET) // DC -->0
#define LCD_DC_Set() HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET)   // DC -->1

#define LCD_CS_Clr() HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET) // CS -->0
#define LCD_CS_Set() HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET)   // CS -->1

SPI_HandleTypeDef* TFT_SPI = &hspi1;

/**
 * @brief SPI发送数据
 *
 * @param dat 要发送的数据
 * @param mode 发送模式,0:软件SPI,1:硬件SPI
 */
HAL_StatusTypeDef LCD_SPI_Writ_Bus(uint8_t *data, uint16_t size)
{
	if(lcd.lcdMode.spiMode == 0) //SOFTWARE_SPI
		return  HAL_ERROR;
	else	//HARDWARE_SPI
		if (lcd.lcdMode.dmaMode == 0)
			return HAL_SPI_Transmit(TFT_SPI, data, size, 100);
		else
		{
			HAL_SPI_Transmit_DMA(TFT_SPI, data, size);
			// 等待传输完成
			while (HAL_SPI_GetState(TFT_SPI) != HAL_SPI_STATE_READY) {}
			return HAL_OK;
		}
}

/**
 * @brief LCD发送uint8_t数据
 *
 * @param dat 要发送的数据
 */
HAL_StatusTypeDef drv_LcdWRData8(uint8_t dat)
{
	HAL_StatusTypeDef status;
	LCD_DC_Set(); // 写数据
	LCD_CS_Clr(); // 片选
	status = LCD_SPI_Writ_Bus(&dat, 1);
	LCD_CS_Set(); // 片选
	return status;
}

/**
 * @brief LCD发送uint16_t数据
 *
 * @param dat 要发送的数据
 */
HAL_StatusTypeDef drv_LcdWRData16(uint16_t dat)
{
	HAL_StatusTypeDef status;
	uint8_t buf[2];
	buf[0] = dat >> 8;
	buf[1] = dat & 0xFF;
	LCD_DC_Set(); // 写数据
	LCD_CS_Clr(); // 片选
	status = LCD_SPI_Writ_Bus(buf, 2);
	LCD_CS_Set(); // 片选
	return status;
}

HAL_StatusTypeDef drv_LcdWRDatas(uint8_t *dat, uint16_t len)
{
	HAL_StatusTypeDef status;
	LCD_DC_Set(); // 写数据
	LCD_CS_Clr(); // 片选
	status = LCD_SPI_Writ_Bus(dat, len);
	LCD_CS_Set(); // 片选
	return status; 
}


/**
 * @brief LCD写寄存器指令
 *
 * @param dat 要发送的寄存器地址
 */
HAL_StatusTypeDef drv_LcdWRReg(uint8_t dat)
{
	HAL_StatusTypeDef status;
	LCD_DC_Clr(); // 写命令
	LCD_CS_Clr(); // 片选
	status = LCD_SPI_Writ_Bus(&dat, 1);
	LCD_CS_Set(); // 片选
	return status;
}

/**
 * @brief LCD设置显示范围
 *
 * @param x1 起点x坐标
 * @param y1 起点y坐标
 * @param x2 终点x坐标
 * @param y2 终点y坐标
 * @note 该函数设置的坐标范围是包含起点和终点的
 */
 HAL_StatusTypeDef drv_LcdAddressSet(uint16_t *x1, uint16_t *y1, uint16_t *x2, uint16_t *y2)
{
	if (USE_HORIZONTAL == 0)
	{
		drv_LcdWRReg(ST7789_CASET); // 列地址设置
		drv_LcdWRData16(*x1);
		drv_LcdWRData16(*x2);
		drv_LcdWRReg(ST7789_RASET); // 行地址设置
		drv_LcdWRData16(*y1);
		drv_LcdWRData16(*y2);
		drv_LcdWRReg(ST7789_RAMWR); // 储存器写
	}
	else if (USE_HORIZONTAL == 1)
	{
		drv_LcdWRReg(ST7789_CASET); // 列地址设置
		drv_LcdWRData16(*x1);
		drv_LcdWRData16(*x2);
		drv_LcdWRReg(ST7789_RASET); // 行地址设置
		drv_LcdWRData16(*y1);
		drv_LcdWRData16(*y2);
		drv_LcdWRReg(ST7789_RAMWR); // 储存器写
	}
	else if (USE_HORIZONTAL == 2)
	{
		drv_LcdWRReg(ST7789_CASET); // 列地址设置
		drv_LcdWRData16(*x1);
		drv_LcdWRData16(*x2);
		drv_LcdWRReg(ST7789_RASET); // 行地址设置
		drv_LcdWRData16(*y1);
		drv_LcdWRData16(*y2);
		drv_LcdWRReg(ST7789_RAMWR); // 储存器写
	}
	else
	{
		drv_LcdWRReg(ST7789_CASET); // 列地址设置
		drv_LcdWRData16(*x1);
		drv_LcdWRData16(*x2);
		drv_LcdWRReg(ST7789_RASET); // 行地址设置
		drv_LcdWRData16(*y1);
		drv_LcdWRData16(*y2);
		drv_LcdWRReg(ST7789_RAMWR); // 储存器写
	}
	return HAL_OK;
}

/**
 * @brief LCD初始化
 *
 */
HAL_StatusTypeDef drv_LcdInit(void)//LCD初始化
{
	LCD_RES_Clr(); // 复位
	HAL_Delay(100);
	LCD_RES_Set();
	HAL_Delay(100);

	LCD_BLK_Set(); // 打开背光
	HAL_Delay(100);

	//************* Start Initial Sequence **********//
	drv_LcdWRReg(0x11);
	HAL_Delay(100);	  // Delay 120ms
	drv_LcdWRReg(0X36); // Memory Access Control
	if (USE_HORIZONTAL == 0)
		drv_LcdWRData8(0x00);
	else if (USE_HORIZONTAL == 1)
		drv_LcdWRData8(0xC0);
	else if (USE_HORIZONTAL == 2)
		drv_LcdWRData8(0x70);
	else
		drv_LcdWRData8(0xA0);
	drv_LcdWRReg(0X3A);
	drv_LcdWRData8(0X05);
	//--------------------------------ST7789S Frame rate setting-------------------------

	drv_LcdWRReg(0xb2);
	drv_LcdWRData8(0x0c);
	drv_LcdWRData8(0x0c);
	drv_LcdWRData8(0x00);
	drv_LcdWRData8(0x33);
	drv_LcdWRData8(0x33);
	drv_LcdWRReg(0xb7);
	drv_LcdWRData8(0x35);
	//---------------------------------ST7789S Power setting-----------------------------

	drv_LcdWRReg(0xbb);
	drv_LcdWRData8(0x35);
	drv_LcdWRReg(0xc0);
	drv_LcdWRData8(0x2c);
	drv_LcdWRReg(0xc2);
	drv_LcdWRData8(0x01);
	drv_LcdWRReg(0xc3);
	drv_LcdWRData8(0x13);
	drv_LcdWRReg(0xc4);
	drv_LcdWRData8(0x20);
	drv_LcdWRReg(0xc6);
	drv_LcdWRData8(0x0f);
	drv_LcdWRReg(0xca);
	drv_LcdWRData8(0x0f);
	drv_LcdWRReg(0xc8);
	drv_LcdWRData8(0x08);
	drv_LcdWRReg(0x55);
	drv_LcdWRData8(0x90);
	drv_LcdWRReg(0xd0);
	drv_LcdWRData8(0xa4);
	drv_LcdWRData8(0xa1);
	//--------------------------------ST7789S gamma setting------------------------------
	drv_LcdWRReg(0xe0);
	drv_LcdWRData8(0xd0);
	drv_LcdWRData8(0x00);
	drv_LcdWRData8(0x06);
	drv_LcdWRData8(0x09);
	drv_LcdWRData8(0x0b);
	drv_LcdWRData8(0x2a);
	drv_LcdWRData8(0x3c);
	drv_LcdWRData8(0x55);
	drv_LcdWRData8(0x4b);
	drv_LcdWRData8(0x08);
	drv_LcdWRData8(0x16);
	drv_LcdWRData8(0x14);
	drv_LcdWRData8(0x19);
	drv_LcdWRData8(0x20);
	drv_LcdWRReg(0xe1);
	drv_LcdWRData8(0xd0);
	drv_LcdWRData8(0x00);
	drv_LcdWRData8(0x06);
	drv_LcdWRData8(0x09);
	drv_LcdWRData8(0x0b);
	drv_LcdWRData8(0x29);
	drv_LcdWRData8(0x36);
	drv_LcdWRData8(0x54);
	drv_LcdWRData8(0x4b);
	drv_LcdWRData8(0x0d);
	drv_LcdWRData8(0x16);
	drv_LcdWRData8(0x14);
	drv_LcdWRData8(0x21);
	drv_LcdWRData8(0x20);
	drv_LcdWRReg(ST7789_DISPON);
	return HAL_OK;
}

#endif
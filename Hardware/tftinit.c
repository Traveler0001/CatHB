// #include "tftinit.h"
// #include "st7789.h"
// #include "spi.h"
// // #include "mid_timer.h"

// #if 1

// SPI_HandleTypeDef* TFT_SPI = &hspi1;

// /**
//  * @brief SPI发送数据
//  *
//  * @param dat 要发送的数据
//  */
// void LCD_Writ_Bus(uint8_t dat)
// {
// // tim3_it_stop();
// #ifdef SOFTWARE_SPI

// #endif // SOFTWARE_SPI

// #ifdef HARDWARE_SPI
// 	// 发送数据
// 	HAL_SPI_Transmit(TFT_SPI, &dat, sizeof(dat), HAL_MAX_DELAY);
// 	// 等待SPI总线空闲
// 	// while (__HAL_SPI_GET_FLAG(TFT_SPI, SPI_FLAG_BSY) == SET)
// 	// 	;
// #endif // HARDWARE_SPI
// 	   // tim3_it_start();
// }

// /**
//  * @brief LCD发送uint8_t数据
//  *
//  * @param dat 要发送的数据
//  */
// void LCD_WR_DATA8(uint8_t dat)
// {
// 	LCD_DC_Set(); // 写数据
// 	LCD_CS_Clr(); // 片选
// 	LCD_Writ_Bus(dat);
// 	LCD_CS_Set(); // 片选
// }

// /**
//  * @brief LCD发送uint16_t数据
//  *
//  * @param dat 要发送的数据
//  */
// void LCD_WR_DATA(uint16_t dat)
// {
// 	LCD_DC_Set(); // 写数据
// 	LCD_CS_Clr(); // 片选
// 	LCD_Writ_Bus(dat >> 8);
// 	LCD_Writ_Bus(dat);
// 	LCD_CS_Set(); // 片选
// }

// void LCD_WR_DATAS(uint8_t *dat, uint16_t len)
// {
// 	LCD_DC_Set(); // 写数据
// 	LCD_CS_Clr(); // 片选
// 	HAL_SPI_Transmit_DMA(TFT_SPI, dat, len);
// 	// LCD_Writ_Bus(dat >> 8);
// 	// LCD_Writ_Bus(dat);
// 	LCD_CS_Set(); // 片选
// }


// /**
//  * @brief LCD写寄存器指令
//  *
//  * @param dat 要发送的寄存器地址
//  */
// void LCD_WR_REG(uint8_t dat)
// {
// 	LCD_DC_Clr(); // 写命令
// 	LCD_CS_Clr(); // 片选
// 	LCD_Writ_Bus(dat);
// 	LCD_CS_Set(); // 片选
// }

// /**
//  * @brief LCD设置显示范围
//  *
//  * @param x1 起点x坐标
//  * @param y1 起点y坐标
//  * @param x2 终点x坐标
//  * @param y2 终点y坐标
//  * @note 该函数设置的坐标范围是包含起点和终点的
//  */
// void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
// {
// 	if (USE_HORIZONTAL == 0)
// 	{
// 		LCD_WR_REG(ST7789_CASET); // 列地址设置
// 		LCD_WR_DATA(x1);
// 		LCD_WR_DATA(x2);
// 		LCD_WR_REG(ST7789_RASET); // 行地址设置
// 		LCD_WR_DATA(y1);
// 		LCD_WR_DATA(y2);
// 		LCD_WR_REG(ST7789_RAMWR); // 储存器写
// 	}
// 	else if (USE_HORIZONTAL == 1)
// 	{
// 		LCD_WR_REG(ST7789_CASET); // 列地址设置
// 		LCD_WR_DATA(x1);
// 		LCD_WR_DATA(x2);
// 		LCD_WR_REG(ST7789_RASET); // 行地址设置
// 		LCD_WR_DATA(y1);
// 		LCD_WR_DATA(y2);
// 		LCD_WR_REG(ST7789_RAMWR); // 储存器写
// 	}
// 	else if (USE_HORIZONTAL == 2)
// 	{
// 		LCD_WR_REG(ST7789_CASET); // 列地址设置
// 		LCD_WR_DATA(x1);
// 		LCD_WR_DATA(x2);
// 		LCD_WR_REG(ST7789_RASET); // 行地址设置
// 		LCD_WR_DATA(y1);
// 		LCD_WR_DATA(y2);
// 		LCD_WR_REG(ST7789_RAMWR); // 储存器写
// 	}
// 	else
// 	{
// 		LCD_WR_REG(ST7789_CASET); // 列地址设置
// 		LCD_WR_DATA(x1);
// 		LCD_WR_DATA(x2);
// 		LCD_WR_REG(ST7789_RASET); // 行地址设置
// 		LCD_WR_DATA(y1);
// 		LCD_WR_DATA(y2);
// 		LCD_WR_REG(ST7789_RAMWR); // 储存器写
// 	}
// }

// /**
//  * @brief LCD初始化
//  *
//  */
// void LCD_Init(void)
// {

// 	// LCD_GPIO_Init();//初始化GPIO

// 	LCD_RES_Clr(); // 复位
// 	HAL_Delay(100);
// 	LCD_RES_Set();
// 	HAL_Delay(100);

// 	LCD_BLK_Set(); // 打开背光
// 	HAL_Delay(100);

// 	//************* Start Initial Sequence **********//
// 	LCD_WR_REG(0x11);
// 	HAL_Delay(100);	  // Delay 120ms
// 	LCD_WR_REG(0X36); // Memory Access Control
// 	if (USE_HORIZONTAL == 0)
// 		LCD_WR_DATA8(0x00);
// 	else if (USE_HORIZONTAL == 1)
// 		LCD_WR_DATA8(0xC0);
// 	else if (USE_HORIZONTAL == 2)
// 		LCD_WR_DATA8(0x70);
// 	else
// 		LCD_WR_DATA8(0xA0);
// 	LCD_WR_REG(0X3A);
// 	LCD_WR_DATA8(0X05);
// 	//--------------------------------ST7789S Frame rate setting-------------------------

// 	LCD_WR_REG(0xb2);
// 	LCD_WR_DATA8(0x0c);
// 	LCD_WR_DATA8(0x0c);
// 	LCD_WR_DATA8(0x00);
// 	LCD_WR_DATA8(0x33);
// 	LCD_WR_DATA8(0x33);
// 	LCD_WR_REG(0xb7);
// 	LCD_WR_DATA8(0x35);
// 	//---------------------------------ST7789S Power setting-----------------------------

// 	LCD_WR_REG(0xbb);
// 	LCD_WR_DATA8(0x35);
// 	LCD_WR_REG(0xc0);
// 	LCD_WR_DATA8(0x2c);
// 	LCD_WR_REG(0xc2);
// 	LCD_WR_DATA8(0x01);
// 	LCD_WR_REG(0xc3);
// 	LCD_WR_DATA8(0x13);
// 	LCD_WR_REG(0xc4);
// 	LCD_WR_DATA8(0x20);
// 	LCD_WR_REG(0xc6);
// 	LCD_WR_DATA8(0x0f);
// 	LCD_WR_REG(0xca);
// 	LCD_WR_DATA8(0x0f);
// 	LCD_WR_REG(0xc8);
// 	LCD_WR_DATA8(0x08);
// 	LCD_WR_REG(0x55);
// 	LCD_WR_DATA8(0x90);
// 	LCD_WR_REG(0xd0);
// 	LCD_WR_DATA8(0xa4);
// 	LCD_WR_DATA8(0xa1);
// 	//--------------------------------ST7789S gamma setting------------------------------
// 	LCD_WR_REG(0xe0);
// 	LCD_WR_DATA8(0xd0);
// 	LCD_WR_DATA8(0x00);
// 	LCD_WR_DATA8(0x06);
// 	LCD_WR_DATA8(0x09);
// 	LCD_WR_DATA8(0x0b);
// 	LCD_WR_DATA8(0x2a);
// 	LCD_WR_DATA8(0x3c);
// 	LCD_WR_DATA8(0x55);
// 	LCD_WR_DATA8(0x4b);
// 	LCD_WR_DATA8(0x08);
// 	LCD_WR_DATA8(0x16);
// 	LCD_WR_DATA8(0x14);
// 	LCD_WR_DATA8(0x19);
// 	LCD_WR_DATA8(0x20);
// 	LCD_WR_REG(0xe1);
// 	LCD_WR_DATA8(0xd0);
// 	LCD_WR_DATA8(0x00);
// 	LCD_WR_DATA8(0x06);
// 	LCD_WR_DATA8(0x09);
// 	LCD_WR_DATA8(0x0b);
// 	LCD_WR_DATA8(0x29);
// 	LCD_WR_DATA8(0x36);
// 	LCD_WR_DATA8(0x54);
// 	LCD_WR_DATA8(0x4b);
// 	LCD_WR_DATA8(0x0d);
// 	LCD_WR_DATA8(0x16);
// 	LCD_WR_DATA8(0x14);
// 	LCD_WR_DATA8(0x21);
// 	LCD_WR_DATA8(0x20);
// 	LCD_WR_REG(ST7789_DISPON);
// }

// #endif
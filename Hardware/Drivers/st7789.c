// /*
//  * Copyright (C) 2015-2020 Alibaba Group Holding Limited
//  */
// #include "stdio.h"
// #include "gpio.h"
// #include "spi.h"
// #include "st7789.h"
// #include "dma.h"
// #include "usart.h"

// // #define FREERTOS

// // #ifdef FREERTOS
// // #include "freertos.h"
// // #include "task.h"
// // #endif // FREERTOS

// /**
//  * @brief SPI总线写入数据
//  *
//  * @param data 指向要写入的数据的指针
//  * @param size 要写入的数据大小
//  */
// void spi_write(const uint8_t *data, uint32_t size)
// {
// 	uint32_t dmaTime = 0; // DMA传输时间

// 	uint32_t rest = size;
// 	uint32_t start = 0;

// #ifdef FREERTOS				  // 如果使用FreeRTOS,则使用DMA传输
// 	if (size > SPI_MAX_BLOCK) // 如果数据大小超过最大块大小
// 	{
// 		while (rest > SPI_MAX_BLOCK)
// 		{
// 			HAL_SPI_Transmit_DMA(&hspi2, (const uint8_t *)(data + start), SPI_MAX_BLOCK);
// 			start += SPI_MAX_BLOCK;
// 			rest -= SPI_MAX_BLOCK;
// 			dmaTime = (float)SPI_MAX_BLOCK * 0.0004f;
// 			vTaskDelay(dmaTime);
// 		}
// 		HAL_SPI_Transmit_DMA(&hspi2, (const uint8_t *)(data + start), rest);
// 		dmaTime = (float)rest * 0.0004f;
// 		vTaskDelay(dmaTime);
// 	}
// 	else
// 	{
// 		HAL_SPI_Transmit_DMA(&hspi2, (const uint8_t *)data, (uint16_t)size);
// 		dmaTime = (float)size * 0.0004f;
// 		vTaskDelay(dmaTime);
// 	}
// #else // 如果不使用FreeRTOS,则使用阻塞传输
// 	if (size > SPI_MAX_BLOCK) // 如果数据大小超过最大块大小
// 	{
// 		while (rest > SPI_MAX_BLOCK)
// 		{
// 			HAL_SPI_Transmit(&hspi2, (const uint8_t *)(data + start), SPI_MAX_BLOCK, HAL_MAX_DELAY);
// 			start += SPI_MAX_BLOCK;
// 			rest -= SPI_MAX_BLOCK;
// 		}
// 		HAL_SPI_Transmit(&hspi2, (const uint8_t *)(data + start), rest, HAL_MAX_DELAY);
// 	}
// 	else
// 	{
// 		HAL_SPI_Transmit(&hspi2, (const uint8_t *)data, (uint16_t)size, HAL_MAX_DELAY);
// 	}
// #endif
// }

// /**
//  * @brief 写入命令到ST7789显示器
//  *
//  * @param c 要写入的命令
//  */
// static inline void write_command(uint8_t c)
// {
// 	DC_Command();
// 	spi_write(&c, sizeof(c));
// }

// /**
//  * @brief 写入数据到ST7789显示器
//  *
//  * @param c 要写入的数据
//  */
// static inline void write_data(uint8_t c)
// {
// 	DC_Data();
// 	spi_write(&c, 1);
// }

// /**
//  * @brief 写入多个数据到ST7789显示器
//  *
//  * @param data 指向要写入的数据的指针
//  */
// static inline void wirte_datas(const uint8_t *data)
// {
// 	DC_Data();
// 	spi_write(data, sizeof(data));
// }

// /**
//  * @brief 设置地址窗口
//  *
//  * @param x_0 起始X坐标
//  * @param y_0 起始Y坐标
//  * @param x_1 结束X坐标
//  * @param y_1 结束Y坐标
//  */
// void st7789_set_addr_window(uint16_t x_0, uint16_t y_0, uint16_t x_1, uint16_t y_1)
// {
// 	uint8_t data[4];

// 	if (x_0 > x_1)
// 	{
// 		x_0 = x_0 ^ x_1;
// 		x_1 = x_0 ^ x_1;
// 		x_0 = x_0 ^ x_1;
// 	}
// 	if (y_0 > y_1)
// 	{
// 		y_0 = y_0 ^ y_1;
// 		y_1 = y_0 ^ y_1;
// 		y_0 = y_0 ^ y_1;
// 	}

// 	write_command(ST7789_CASET);
// 	data[0] = x_0 >> 8;
// 	data[1] = x_0;
// 	data[2] = x_1 >> 8;
// 	data[3] = x_1;
// 	wirte_datas(data);
// 	write_command(ST7789_RASET);
// 	data[0] = y_0 >> 8;
// 	data[1] = y_0;
// 	data[2] = y_1 >> 8;
// 	data[3] = y_1;
// 	wirte_datas(data);
// 	write_command(ST7789_RAMWR);
// }

// /**
//  * @brief 初始化ST7789显示器的命令列表
//  *
//  */
// static void command_list(void)
// {
// #ifdef FREERTOS
// 	// 复位
// 	HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_SET);
// 	vTaskDelay(1);
// 	HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_RESET);
// 	vTaskDelay(10);
// 	HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_SET);
// 	vTaskDelay(120);
// #else
// 	HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_SET);
// 	HAL_Delay(1);
// 	HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_RESET);
// 	HAL_Delay(10);
// 	HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_SET);
// 	HAL_Delay(120);
// #endif // FREERTOS

// 	write_command(ST7789_MADCTL);  // 内存访问控制
// 	write_data(ST7789_MADCTL_RGB); // RGB顺序

// 	write_command(ST7789_COLMOD); // 像素格式设置
// 	write_data(0x05);			  // 16位色深

// 	write_command(0xb2); // Porch设置
// 	write_data(0x0c);
// 	write_data(0x0c);
// 	write_data(0x00);
// 	write_data(0x33);
// 	write_data(0x33);

// 	// 注释使用中问
// 	write_command(0xb7); // gata控制
// 	write_data(0x70);

// 	write_command(0xbb); // VCOM设置
// 	write_data(0x21);

// 	write_command(0xc0);
// 	write_data(0x2c);

// 	write_command(0xc2);
// 	write_data(0x01);

// 	write_command(0xc3);
// 	write_data(0x0B);

// 	write_command(0xc4);
// 	write_data(0x27);

// 	write_command(0xc6);
// 	write_data(0x0f);

// 	write_command(0xd0);
// 	write_data(0xa4);
// 	write_data(0xA1);

// 	write_command(0xe0);
// 	write_data(0xD0);
// 	write_data(0x06);
// 	write_data(0x0B);
// 	write_data(0x09);
// 	write_data(0x08);
// 	write_data(0x30);
// 	write_data(0x30);
// 	write_data(0x5B);
// 	write_data(0x4B);
// 	write_data(0x18);
// 	write_data(0x14);
// 	write_data(0x14);
// 	write_data(0x2C);
// 	write_data(0x32);

// 	write_command(0xe1);
// 	write_data(0xD0);
// 	write_data(0x05);
// 	write_data(0x0A);
// 	write_data(0x0A);
// 	write_data(0x07);
// 	write_data(0x28);
// 	write_data(0x32);
// 	write_data(0x2C);
// 	write_data(0x49);
// 	write_data(0x18);
// 	write_data(0x13);
// 	write_data(0x13);
// 	write_data(0x2C);
// 	write_data(0x33);

// 	write_command(0x21);

// 	write_command(0x2A);
// 	write_data(0x00);
// 	write_data(0x00);
// 	write_data(0x00);
// 	write_data(0xEF);

// 	write_command(0x2B);
// 	write_data(0x00);
// 	write_data(0x00);
// 	write_data(0x01);
// 	write_data(0x3F);

// 	write_command(0x11);
// #ifdef FREERTOS
// 	vTaskDelay(120);
// #else
// 	HAL_Delay(120);
// #endif // FREERTOS

// 	write_command(0x29);
// 	write_command(0x2c);
// }

// /**
//  * @brief 初始化硬件
//  *
//  */
// static void hardware_init(void)
// {
// 	// int32_t ret = 0;
// 	uint8_t str[30] = {0};

// 	HAL_GPIO_WritePin(TFT_BK_GPIO_Port, TFT_BK_Pin, GPIO_PIN_SET);

// #ifdef FREERTOS
// 	sprintf((char *)str, "hardware_init START\r\n");
// 	HAL_UART_Transmit(&huart1, str, sizeof(str), HAL_MAX_DELAY);
// 	vTaskDelay(5);
// 	HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_RESET);
// 	vTaskDelay(500);
// 	HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_SET);
// 	sprintf((char *)str, "hal_gpio_init done\r\n");
// 	HAL_UART_Transmit(&huart1, str, sizeof(str), HAL_MAX_DELAY);
// 	vTaskDelay(5);
// #else
// 	sprintf((char *)str, "hardware_init START\r\n");
// 	HAL_UART_Transmit(&huart1, str, sizeof(str), HAL_MAX_DELAY);
// 	HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_RESET);
// 	HAL_Delay(500);
// 	HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_SET);
// 	sprintf((char *)str, "hal_gpio_init done\r\n");
// 	HAL_UART_Transmit(&huart1, str, sizeof(str), HAL_MAX_DELAY);
// #endif // FREERTOS
// }

// /**
//  * @brief 设置显示器的旋转角度
//  * @param rotation 旋转角度，0、90、180或270度
//  */
// void st7789_rotation_set(uint16_t rotation)
// {
// 	write_command(ST7789_MADCTL);
// 	switch (rotation)
// 	{
// 	case 0:
// 		write_data(ST7789_MADCTL_MX | ST7789_MADCTL_RGB);
// 		break;
// 	case 90:
// 		write_data(ST7789_MADCTL_MV | ST7789_MADCTL_MX | ST7789_MADCTL_RGB);
// 		break;
// 	case 180:
// 		write_data(ST7789_MADCTL_MY | ST7789_MADCTL_RGB);
// 		break;
// 	case 270:
// 		write_data(ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_MV |
// 				   ST7789_MADCTL_RGB);
// 		break;
// 	default:
// 		break;
// 	}
// }

// /**
//  * @brief 初始化ST7789显示器
//  *
//  */
// void st7789_init(void)
// {
// 	hardware_init();

// 	command_list();

// 	st7789_rotation_set(90);
// 	write_command(ST7789_DISPON); // 开启显示
// }

// // static void st7789_uninit(void)
// // {
// // 	;
// // }

// /**
//  * @brief 设置反色显示
//  *
//  * @param invert 1表示开启反色显示，0表示关闭反色显示
//  */
// void st7789_display_invert(uint8_t invert)
// {
// 	write_command(invert ? ST7789_INVON : ST7789_INVOFF);
// }

// /**
//  * @brief 进入睡眠模式
//  *
//  */
// void st7789_EnterSleep(void)
// {
// 	write_command(ST7789_DISPOFF);
// #ifdef FREERTOS

// 	vTaskDelay(20);
// #else
// 	HAL_Delay(20);

// #endif // FREERTOS
// 	write_command(ST7789_SLPIN);
// }

// /**
//  * @brief 退出睡眠模式
//  *
//  */
// void st7789_ExitSleep(void)
// {
// 	write_command(ST7789_SLPOUT);
// #ifdef FREERTOS
// 	vTaskDelay(50);
// #else
// 	HAL_Delay(120);
// #endif // FREERTOS
// 	write_command(ST7789_DISPON);
// }

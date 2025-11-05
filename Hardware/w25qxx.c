
#include "w25qxx.h"
#include "gpio.h"
#include <stdio.h>
#include "tftlcd.h"
// #include "lvgl.h"
#include "ff.h"

#if 0

uint16_t BSP_W25QXX_TYPE = W25Q128; // 默认是W25Q128
#define DUMMY_BYTE 0xFF
#define fspi hspi3

uint8_t SPI_FLASH_WriteByte(uint8_t data)
{
	HAL_SPI_Transmit(&fspi, &data, 1, 500);
	// while (HAL_DMA_GetState(fspi.hdmarx) != HAL_DMA_STATE_READY)
	// 	;
	return HAL_OK;
	// return uc_read;
}

uint8_t SPI_FLASH_ReadByte()
{
	uint8_t data;
	HAL_SPI_Receive(&fspi, &data, 1, 1000);
	// while (HAL_DMA_GetState(fspi.hdmarx) != HAL_DMA_STATE_READY)
	// 	;
	return data;
	// return uc_read;
}

uint8_t SPI_FLASH_WriteBytes(uint8_t *pBuffer, uint16_t NumByteToRead)
{
	HAL_SPI_Transmit(&fspi, pBuffer, NumByteToRead, 500);
	return HAL_OK;
}

uint8_t SPI_FLASH_ReadBytes(uint8_t *pBuffer, uint16_t NumByteToRead)
{
	HAL_SPI_Receive(&fspi, pBuffer, NumByteToRead, 1000);
	return HAL_OK;
}

uint8_t SPI_FLASH_WriteByte_DMA(uint8_t data)
{
	while (HAL_DMA_GetState(fspi.hdmatx) == HAL_DMA_STATE_READY)
		;
	HAL_SPI_Transmit_DMA(&fspi, &data, 1);
	while (HAL_DMA_GetState(fspi.hdmatx) != HAL_DMA_STATE_READY)
		;
	return HAL_OK;
}

uint8_t SPI_FLASH_ReadByte_DMA()
{
	uint8_t data;
	while (HAL_DMA_GetState(fspi.hdmarx) == HAL_DMA_STATE_READY)
		;
	HAL_SPI_Receive_DMA(&fspi, &data, 1);
	while (HAL_DMA_GetState(fspi.hdmarx) != HAL_DMA_STATE_READY)
		;
	return data;
}

uint8_t SPI_FLASH_WriteBytes_DMA(uint8_t *pBuffer, uint16_t NumByteToRead)
{
	while (HAL_DMA_GetState(fspi.hdmatx) == HAL_DMA_STATE_READY)
		;
	HAL_SPI_Transmit_DMA(&fspi, pBuffer, NumByteToRead);
	while (HAL_DMA_GetState(fspi.hdmatx) != HAL_DMA_STATE_READY)
		;
	return HAL_OK;
}

uint8_t SPI_FLASH_ReadBytes_DMA(uint8_t *pBuffer, uint16_t NumByteToRead)
{
	while (HAL_DMA_GetState(fspi.hdmarx) == HAL_DMA_STATE_READY)
		;
	HAL_SPI_Receive_DMA(&fspi, pBuffer, NumByteToRead);
	while (HAL_DMA_GetState(fspi.hdmarx) != HAL_DMA_STATE_READY)
		;
	return HAL_OK;
}



// 4Kbytes为一个Sector
// 16个扇区为1个Block
// W25Q128
// 容量为16M字节,共有128个Block,4096个Sector

// 初始化SPI FLASH的IO口
uint16_t BSP_W25QXX_Init(void)
{
	BSP_W25QXX_TYPE = BSP_W25QXX_ReadID(); // 读取FLASH ID.
	return BSP_W25QXX_TYPE;
	// PRINTF_DEBUG("BSP_W25QXX_TYPE_ID = %X\r\n", BSP_W25QXX_TYPE);
}

// 读取W25QXX的状态寄存器
// BIT7  6   5   4   3   2   1   0
// SPR   RV  TB BP2 BP1 BP0 WEL BUSY
// SPR:默认0,状态寄存器保护位,配合WP使用
// TB,BP2,BP1,BP0:FLASH区域写保护设置
// WEL:写使能锁定
// BUSY:忙标记位(1,忙;0,空闲)
// 默认:0x00
uint8_t BSP_W25QXX_ReadSR(void)
{
	uint8_t byte = 0;
	W25Qx_Enable();								 // 使能器件
	SPI_FLASH_WriteByte(W25X_ReadStatusReg); // 发送读取状态寄存器命令
	byte = SPI_FLASH_ReadByte();		 // 读取一个字节
	W25Qx_Disable();							 // 取消片选
	return byte;
}
// 写W25QXX状态寄存器
// 只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void BSP_W25QXX_Write_SR(uint8_t sr)
{
	W25Qx_Enable();								  // 使能器件
	SPI_FLASH_WriteByte(W25X_WriteStatusReg); // 发送写取状态寄存器命令
	SPI_FLASH_WriteByte(sr);				  // 写入一个字节
	W25Qx_Disable();							  // 取消片选
}
// W25QXX写使能
// 将WEL置位
void BSP_W25QXX_Write_Enable(void)
{
	W25Qx_Enable();							   // 使能器件
	SPI_FLASH_WriteByte(W25X_WriteEnable); // 发送写使能
	W25Qx_Disable();						   // 取消片选
}
// W25QXX写禁止
// 将WEL清零
void BSP_W25QXX_Write_Disable(void)
{
	W25Qx_Enable();								// 使能器件
	SPI_FLASH_WriteByte(W25X_WriteDisable); // 发送写禁止指令
	W25Qx_Disable();							// 取消片选
}
// 读取芯片ID
// 返回值如下:
// 0XEF13,表示芯片型号为W25Q80
// 0XEF14,表示芯片型号为W25Q16
// 0XEF15,表示芯片型号为W25Q32
// 0XEF16,表示芯片型号为W25Q64
// 0XEF17,表示芯片型号为W25Q128
uint16_t BSP_W25QXX_ReadID(void)
{
	uint16_t Temp = 0;
	W25Qx_Enable();
	SPI_FLASH_WriteByte(0x90); // 发送读取ID命令
	SPI_FLASH_WriteByte(0x00);
	SPI_FLASH_WriteByte(0x00);
	SPI_FLASH_WriteByte(0x00);
	Temp |= SPI_FLASH_ReadByte() << 8;
	Temp |= SPI_FLASH_ReadByte();
	W25Qx_Disable();
	return Temp;
}
// 读取SPI FLASH
// 在指定地址开始读取指定长度的数据
// pBuffer:数据存储区
// ReadAddr:开始读取的地址(24bit)
// NumByteToRead:要读取的字节数(最大65535)
void BSP_W25QXX_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	uint16_t i;
	W25Qx_Enable();										  // 使能器件
	SPI_FLASH_WriteByte(W25X_ReadData);				  // 发送读取命令
	SPI_FLASH_WriteByte((uint8_t)((ReadAddr) >> 16)); // 发送24bit地址
	SPI_FLASH_WriteByte((uint8_t)((ReadAddr) >> 8));
	SPI_FLASH_WriteByte((uint8_t)ReadAddr);
	// for (i = 0; i < NumByteToRead; i++)
	// {
	// 	pBuffer[i] = SPI_FLASH_ReadByte(); // 循环读数
	// }
	SPI_FLASH_ReadBytes(pBuffer, NumByteToRead);
	W25Qx_Disable();
}
// SPI在一页(0~65535)内写入少于256个字节的数据
// 在指定地址开始写入最大256字节的数据
// pBuffer:数据存储区
// WriteAddr:开始写入的地址(24bit)
// NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
void BSP_W25QXX_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint16_t i;
	BSP_W25QXX_Write_Enable();							   // SET WEL
	W25Qx_Enable();										   // 使能器件
	SPI_FLASH_WriteByte(W25X_PageProgram);			   // 发送写页命令
	SPI_FLASH_WriteByte((uint8_t)((WriteAddr) >> 16)); // 发送24bit地址
	SPI_FLASH_WriteByte((uint8_t)((WriteAddr) >> 8));
	SPI_FLASH_WriteByte((uint8_t)WriteAddr);
	// for (i = 0; i < NumByteToWrite; i++)
	// 	SPI_FLASH_ReadWriteByte(pBuffer[i]); // 循环写数
	SPI_FLASH_WriteBytes(pBuffer, NumByteToWrite);
	W25Qx_Disable();						 // 取消片选
	BSP_W25QXX_Wait_Busy();					 // 等待写入结束
}
// 无检验写SPI FLASH
// 必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
// 具有自动换页功能
// 在指定地址开始写入指定长度的数据,但是要确保地址不越界!
// pBuffer:数据存储区
// WriteAddr:开始写入的地址(24bit)
// NumByteToWrite:要写入的字节数(最大65535)
// CHECK OK
void BSP_W25QXX_Write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint16_t pageremain;
	pageremain = W25Q128FV_PAGE_SIZE - WriteAddr % W25Q128FV_PAGE_SIZE; // 单页剩余的字节数
	if (NumByteToWrite <= pageremain)
		pageremain = NumByteToWrite; // 不大于256个字节
	while (1)
	{
		BSP_W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
		if (NumByteToWrite == pageremain)
			break; // 写入结束了
		else	   // NumByteToWrite>pageremain
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;

			NumByteToWrite -= pageremain; // 减去已经写入了的字节数
			if (NumByteToWrite > W25Q128FV_PAGE_SIZE)
				pageremain = W25Q128FV_PAGE_SIZE; // 一次可以写入256个字节
			else
				pageremain = NumByteToWrite; // 不够256个字节了
		}
	};
}
// 写SPI FLASH
// 在指定地址开始写入指定长度的数据
// 该函数带擦除操作!
// pBuffer:数据存储区
// WriteAddr:开始写入的地址(24bit)
// NumByteToWrite:要写入的字节数(最大65535)
uint8_t BSP_W25QXX_BUFFER[4096];
void BSP_W25QXX_Write(const uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;
	uint16_t i;
	uint8_t *BSP_W25QXX_BUF;
	BSP_W25QXX_BUF = BSP_W25QXX_BUFFER;
	secpos = WriteAddr / W25Q128FV_SECTOR_SIZE; // 扇区地址
	secoff = WriteAddr % W25Q128FV_SECTOR_SIZE; // 在扇区内的偏移
	secremain = W25Q128FV_SECTOR_SIZE - secoff; // 扇区剩余空间大小
	if (NumByteToWrite <= secremain)
		secremain = NumByteToWrite; // 不大于4096个字节
	while (1)
	{
		BSP_W25QXX_Read(BSP_W25QXX_BUF, secpos * W25Q128FV_SECTOR_SIZE, W25Q128FV_SECTOR_SIZE); // 读出整个扇区的内容
		for (i = 0; i < secremain; i++)																  // 校验数据
		{
			if (BSP_W25QXX_BUF[secoff + i] != 0XFF)
				break; // 需要擦除
		}
		if (i < secremain) // 需要擦除
		{
			BSP_W25QXX_Erase_Sector(secpos); // 擦除这个扇区
			for (i = 0; i < secremain; i++)	 // 复制
			{
				BSP_W25QXX_BUF[i + secoff] = pBuffer[i];
			}
			BSP_W25QXX_Write_NoCheck(BSP_W25QXX_BUF, secpos * W25Q128FV_SECTOR_SIZE, W25Q128FV_SECTOR_SIZE); // 写入整个扇区
		}
		else
			BSP_W25QXX_Write_NoCheck((uint8_t *)pBuffer, WriteAddr, secremain); // 写已经擦除了的,直接写入扇区剩余区间.

		if (NumByteToWrite == secremain)
			break; // 写入结束了
		else	   // 写入未结束
		{
			secpos++;	// 扇区地址增1
			secoff = 0; // 偏移位置为0

			pBuffer += secremain;		 // 指针偏移
			WriteAddr += secremain;		 // 写地址偏移
			NumByteToWrite -= secremain; // 字节数递减
			if (NumByteToWrite > 4096)
				secremain = 4096; // 下一个扇区还是写不完
			else
				secremain = NumByteToWrite; // 下一个扇区可以写完了
		}
	};
}
// 擦除整个芯片
// 等待时间超长...
void BSP_W25QXX_Erase_Chip(void)
{
	BSP_W25QXX_Write_Enable(); // SET WEL
	BSP_W25QXX_Wait_Busy();
	W25Qx_Enable();							 // 使能器件
	SPI_FLASH_WriteByte(W25X_ChipErase); // 发送片擦除命令
	W25Qx_Disable();						 // 取消片选
	BSP_W25QXX_Wait_Busy();					 // 等待芯片擦除结束
}
// 擦除一个扇区   4Kb
// Dst_Addr:扇区地址 根据实 际容量设置
// 擦除一个山区的最少时间:150ms
void BSP_W25QXX_Erase_Sector(uint32_t Dst_Addr)
{
	// 监视falsh擦除情况,测试用
	// 	printf("fe:%x\r\n",Dst_Addr);
	Dst_Addr *= W25Q128FV_SECTOR_SIZE;
	BSP_W25QXX_Write_Enable(); // SET WEL
	BSP_W25QXX_Wait_Busy();
	W25Qx_Enable();										  // 使能器件
	SPI_FLASH_WriteByte(W25X_SectorErase);			  // 发送扇区擦除指令
	SPI_FLASH_WriteByte((uint8_t)((Dst_Addr) >> 16)); // 发送24bit地址
	SPI_FLASH_WriteByte((uint8_t)((Dst_Addr) >> 8));
	SPI_FLASH_WriteByte((uint8_t)Dst_Addr);
	W25Qx_Disable();		// 取消片选
	BSP_W25QXX_Wait_Busy(); // 等待擦除完成
}
// 等待空闲
void BSP_W25QXX_Wait_Busy(void)
{
	while ((BSP_W25QXX_ReadSR() & 0x01) == 0x01)
		; // 等待BUSY位清空
}
// 进入掉电模式
void BSP_W25QXX_PowerDown(void)
{
	W25Qx_Enable();							 // 使能器件
	SPI_FLASH_WriteByte(W25X_PowerDown); // 发送掉电命令
	W25Qx_Disable();						 // 取消片选
	HAL_Delay(1);							 // 等待TPD
}
// 唤醒
void BSP_W25QXX_WAKEUP(void)
{
	W25Qx_Enable();									// 使能器件
	SPI_FLASH_WriteByte(W25X_ReleasePowerDown); //  send W25X_PowerDown command 0xAB
	W25Qx_Disable();								// 取消片选
	HAL_Delay(1);									// 等待TRES1
}

// void BSP_W25QXX_Test(void)
// {
// 	uint16_t FLASHID = BSP_W25QXX_ReadID(); // Read Flash ID
// 	int8_t str[20] = {0};
// 	// __Select_Flash();
// 	// HAL_UART_Transmit(&huart1, str, sizeof(str), HAL_MAX_DELAY);
// 	printf("Flash ID: 0x%04X", FLASHID);
// 	sprintf(str, "Flash ID: 0x%04X", FLASHID);
// 	LCD_ShowString(0, 0, str, BLACK, WHITE, 16, 0);
// }

#if 0
void fatfs_test(void)
{

	FATFS spi_fs;
	FIL fil;
	unsigned int count = 0;
	unsigned char work[4096] = {0};
	unsigned char read_buf[50] = {0};
	unsigned char write_buf[50] = "hello sudaroot\r\n";
	FRESULT retSD;
#if 1
	printf("\r\n ****** FatFs Example ******\r\n");
	/*##-1- Register the file system object to the FatFs module ##############*/
	retSD = f_mount(&spi_fs, "0:", 1);
	if (retSD != FR_OK)
	{
		if (retSD == FR_NO_FILESYSTEM)
		{
			printf("f_mount No file system, start formatting spi-flash\r\n");
			retSD = f_mkfs("0:", 0, 0, work, sizeof(work));
			if (retSD != FR_OK)
			{
				printf("f_mkfs Formatting failed, err = %d\r\n", retSD);

				while (1)
					;
			}
			else
			{
				printf("The format is successful, and the spi-flash is remounted\r\n");
				retSD = f_mount(&spi_fs, "0:", 1);
				if (retSD != FR_OK)
				{
					printf("f_mount An error occurred, err= %d\r\n", retSD);
				}
				else
					printf("The spi-flash file system is mounted successfully\r\n");
			}
		}
		else
		{
			printf("f_mount Other error occurred, err= = %d\r\n", retSD);
			while (1)
				;
		}
	}
	else
		printf("The spi-flash file system is mounted successfully\r\n");
	/*----------------------- 锟侥硷拷系统锟斤拷锟皆ｏ拷写锟斤拷锟斤拷 -----------------------------*/
	printf("\r\n******  File write tests are coming soon... ******\r\n");
	retSD = f_open(&fil, "0:sudaroot.txt", FA_OPEN_ALWAYS | FA_WRITE);
	if (retSD == FR_OK)
	{
		printf("Open/create sudaroot.txt file successfully, write data to the file.\r\n");
		retSD = f_write(&fil, write_buf, sizeof((const char *)write_buf), &count);
		if (retSD != FR_OK)
		{
			printf("f_write Error occurred, err = %d\r\n", retSD);
			printf("Close open sudaroot.txt files\r\n");
			count = 0;
			f_close(&fil);
		}
		else
		{
			printf("The file is written successfully, and the byte data is written:%d\n", count);
			printf("The data written to the file is:\r\n%s\r\n", write_buf);
			printf("Close open sudaroot.txt files\r\n");
			count = 0;
			f_close(&fil);
		}
	}
	else
		printf("Open/create sudaroot.txt file failed, err = %d\r\n", retSD);
	/*------------------- 锟侥硷拷系统锟斤拷锟皆ｏ拷锟斤拷锟斤拷锟斤拷 ------------------------------------*/
	printf("****** File read test coming soon... ******\r\n");
	retSD = f_open(&fil, "0:sudaroot.txt", FA_OPEN_EXISTING | FA_READ);
	if (retSD == FR_OK)
	{
		printf("Open sudaroot.txt file successfully\r\n");
		retSD = f_read(&fil, read_buf, sizeof(read_buf), &count);
		if (retSD != FR_OK)
		{
			printf("f_read Error occurred, err = %d\r\n", retSD);
			printf("Close open sudaroot.txt files\r\n");
			f_close(&fil);
		}
		else
		{
			printf("The file is successfully read, and the byte data is read:%d\n", count);
			printf("The data read to the file is:\r\n%s\r\n", read_buf);
			printf("Close open sudaroot.txt files\r\n");
			f_close(&fil);
		}
	}
	else
		printf("Failed to open sudaroot.txt file, err = %d\r\n", retSD);

#endif // __FATFS_TEST

#if 0
	lv_fs_file_t f;
	lv_fs_res_t res;

	res = lv_fs_open(&f, "0:/sudaroot.txt", LV_FS_MODE_RD);
	if (res != LV_FS_RES_OK)
		printf("Open FileSystem Fail!!! Code %d\r\n", res);
	else
		printf("Open FileSystem Success!!!\r\n");

	unsigned int read_num;

	res = lv_fs_read(&f, read_buf, 8, &read_num);

	if (res != LV_FS_RES_OK || read_num != sizeof(write_buf))
		printf("Read FileSystem Fail!!!Code %d\r\n", res);
	else
		printf("Read FileSystem Success!!!\r\n");
	/* FATFS测试，有LVGL */
	lv_fs_close(&f);
	printf("read_num = %d\r\n", read_num);
	printf(read_buf);
#endif // __lv_fs_test
}
#endif // __Fatfs

// #include "w25qxx.h"
// #include "spi.h"
// #include "gpio.h"

// #define fspi            hspi3 // 使用SPI2接口

// uint16_t W25QXX_TYPE=W25Q256;    //默认是W25Q256

// //4Kbytes为一个Sector
// //16个sector为1个Block
// //W25Q256
// //容量为32M字节,共有512个Block,8192个Sector 


// //SPI速度设置函数
// //SPI速度=fAPB1/分频系数
// //@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
// //fAPB1时钟一般为45Mhz：
// // void SPI5_SetSpeed(uint8_t SPI_BaudRatePrescaler)
// // {
// //     assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
// //     __HAL_SPI_DISABLE(&fspi);            //关闭SPI
// //     fspi.Instance->CR1&=0XFFC7;          //位3-5清零，用来设置波特率
// //     fspi.Instance->CR1|=SPI_BaudRatePrescaler;//设置SPI速度
// //     __HAL_SPI_ENABLE(&fspi);             //使能SPI
    
// // }

// //SPI5 读写一个字节
// //TxData:要写入的字节
// //返回值:读取到的字节
// uint8_t SPI5_ReadWriteByte(uint8_t TxData)
// {
//     // while (SPI_Status() == 1)
//     //     ;
//     uint8_t Rxdata;
//     HAL_SPI_TransmitReceive(&fspi,&TxData,&Rxdata,1, 1000);       
//      return Rxdata;                      //返回收到的数据        
// }

// uint8_t SPI5_ReadWriteByte_DMA(uint8_t RT, uint8_t* pBuffer, uint16_t NumByteToRead)
// {
//     // while (SPI_Status() == 0)
//     //     ;
//     uint8_t status;
//     if (RT == 0) // 发送
//     {
//         // HAL_SPI_Transmit_DMA(&fspi,pBuffer,NumByteToRead);
//         status = HAL_SPI_Transmit(&fspi,pBuffer,NumByteToRead, 1000);
//     }
//     else // 接收
//     {
//         // HAL_SPI_Receive_DMA(&fspi,pBuffer,NumByteToRead);
//         status = HAL_SPI_Receive(&fspi,pBuffer,NumByteToRead, 1000);
//     }

//     return status;
// }

// /**
//  * @brief SPI状态 Busy:1  Idle:0
//  * 
//  * @return uint8_t  0:空闲  1:忙
//  */
// uint8_t SPI_Status(void)
// {
//     // if(HAL_DMA_GetState(fspi.hdmarx) != HAL_DMA_STATE_READY && HAL_DMA_GetState(fspi.hdmatx) != HAL_DMA_STATE_READY)
//     HAL_SPI_GetState(&fspi);
//     if(HAL_SPI_GetState(&fspi) == HAL_SPI_STATE_READY)
//         return 0; // 空闲
//     else
//         return 1; // 忙
// }

// //初始化SPI FLASH的IO口
// void W25QXX_Init(void)
// { 
//     uint8_t temp;
    
//     W25Qx_Disable();                            //SPI FLASH不选中

//     W25QXX_TYPE=W25QXX_ReadID();            //读取FLASH ID.
//     if(W25QXX_TYPE==W25Q256)                //SPI FLASH为W25Q256
//     {
//         temp=W25QXX_ReadSR(3);              //读取状态寄存器3，判断地址模式
//         if((temp&0X01)==0)                    //如果不是4字节地址模式,则进入4字节地址模式
//         {
//             W25Qx_Enable();                     //选中
//             SPI5_ReadWriteByte(W25X_Enable4ByteAddr);//发送进入4字节地址模式指令   
//             W25Qx_Disable();                       //取消片选   
//         }
//     }
// }  

// //读取W25QXX的状态寄存器，W25QXX一共有3个状态寄存器
// //状态寄存器1：
// //BIT7  6   5   4   3   2   1   0
// //SPR   RV  TB BP2 BP1 BP0 WEL BUSY
// //SPR:默认0,状态寄存器保护位,配合WP使用
// //TB,BP2,BP1,BP0:FLASH区域写保护设置
// //WEL:写使能锁定
// //BUSY:忙标记位(1,忙;0,空闲)
// //默认:0x00
// //状态寄存器2：
// //BIT7  6   5   4   3   2   1   0
// //SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
// //状态寄存器3：
// //BIT7      6    5    4   3   2   1   0
// //HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
// //regno:状态寄存器号，范:1~3
// //返回值:状态寄存器值
// uint8_t W25QXX_ReadSR(uint8_t regno)   
// {  
//     uint8_t byte=0,command=0; 
//     switch(regno)
//     {
//         case 1:
//             command=W25X_ReadStatusReg1;    //读状态寄存器1指令
//             break;
//         case 2:
//             command=W25X_ReadStatusReg2;    //读状态寄存器2指令
//             break;
//         case 3:
//             command=W25X_ReadStatusReg3;    //读状态寄存器3指令
//             break;
//         default:
//             command=W25X_ReadStatusReg1;    
//             break;
//     }    
//     W25Qx_Enable();                            //使能器件   
//     SPI5_ReadWriteByte(command);            //发送读取状态寄存器命令    
//     byte=SPI5_ReadWriteByte(0Xff);          //读取一个字节  
//     W25Qx_Disable();                            //取消片选     
//     return byte;   
// } 
// //写W25QXX状态寄存器
// void W25QXX_Write_SR(uint8_t regno,uint8_t sr)   
// {   
//     uint8_t command=0;
//     switch(regno)
//     {
//         case 1:
//             command=W25X_WriteStatusReg1;    //写状态寄存器1指令
//             break;
//         case 2:
//             command=W25X_WriteStatusReg2;    //写状态寄存器2指令
//             break;
//         case 3:
//             command=W25X_WriteStatusReg3;    //写状态寄存器3指令
//             break;
//         default:
//             command=W25X_WriteStatusReg1;    
//             break;
//     }   
//     W25Qx_Enable();                            //使能器件   
//     SPI5_ReadWriteByte(command);            //发送写取状态寄存器命令    
//     SPI5_ReadWriteByte(sr);                 //写入一个字节  
//     W25Qx_Disable();                            //取消片选               
// }   
// //W25QXX写使能    
// //将WEL置位   
// void W25QXX_Write_Enable(void)   
// {
//     W25Qx_Enable();                            //使能器件   
//     SPI5_ReadWriteByte(W25X_WriteEnable);   //发送写使能  
//     W25Qx_Disable();                            //取消片选               
// } 
// //W25QXX写禁止    
// //将WEL清零  
// void W25QXX_Write_Disable(void)   
// {  
//     W25Qx_Enable();                            //使能器件   
//     SPI5_ReadWriteByte(W25X_WriteDisable);  //发送写禁止指令    
//     W25Qx_Disable();                            //取消片选               
// } 

// //读取芯片ID
// //返回值如下:                   
// //0XEF13,表示芯片型号为W25Q80  
// //0XEF14,表示芯片型号为W25Q16    
// //0XEF15,表示芯片型号为W25Q32  
// //0XEF16,表示芯片型号为W25Q64 
// //0XEF17,表示芯片型号为W25Q128       
// //0XEF18,表示芯片型号为W25Q256
// uint16_t W25QXX_ReadID(void)
// {
//     uint16_t Temp = 0;      
//     W25Qx_Enable();                    
//     SPI5_ReadWriteByte(0x90);//发送读取ID命令        
//     SPI5_ReadWriteByte(0x00);         
//     SPI5_ReadWriteByte(0x00);         
//     SPI5_ReadWriteByte(0x00);                     
//     Temp|=SPI5_ReadWriteByte(0xFF)<<8;  
//     Temp|=SPI5_ReadWriteByte(0xFF);     
//     W25Qx_Disable();  
//     HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);                  
//     return Temp;
// }               
// //读取SPI FLASH  
// //在指定地址开始读取指定长度的数据
// //pBuffer:数据存储区
// //ReadAddr:开始读取的地址(24bit)
// //NumByteToRead:要读取的字节数(最大65535)
// void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
// { 
//      uint16_t i;                                               
//     W25Qx_Enable();                            //使能器件   
//     SPI5_ReadWriteByte(W25X_ReadData);      //发送读取命令  
//     if(W25QXX_TYPE==W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
//     {
//         SPI5_ReadWriteByte((uint8_t)((ReadAddr)>>24));    
//     }
//     SPI5_ReadWriteByte((uint8_t)((ReadAddr)>>16));   //发送24bit地址    
//     SPI5_ReadWriteByte((uint8_t)((ReadAddr)>>8));   
//     SPI5_ReadWriteByte((uint8_t)ReadAddr);   
//     for(i=0;i<NumByteToRead;i++)
//     { 
//         pBuffer[i]=SPI5_ReadWriteByte(0XFF);    //循环读数  
//     }
//     // SPI5_ReadWriteByte_DMA(1, pBuffer,NumByteToRead); //使用DMA读数
//     // W25Qx_Disable();                                
// }  
// //SPI在一页(0~65535)内写入少于256个字节的数据
// //在指定地址开始写入最大256字节的数据
// //pBuffer:数据存储区
// //WriteAddr:开始写入的地址(24bit)
// //NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!     
// void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
// {
//      uint16_t i;  
//     W25QXX_Write_Enable();                  //SET WEL 
//     W25Qx_Enable();                            //使能器件   
//     SPI5_ReadWriteByte(W25X_PageProgram);   //发送写页命令   
//     if(W25QXX_TYPE==W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
//     {
//         SPI5_ReadWriteByte((uint8_t)((WriteAddr)>>24)); 
//     }
//     SPI5_ReadWriteByte((uint8_t)((WriteAddr)>>16)); //发送24bit地址    
//     SPI5_ReadWriteByte((uint8_t)((WriteAddr)>>8));   
//     SPI5_ReadWriteByte((uint8_t)WriteAddr);   
//     for(i=0;i<NumByteToWrite;i++)SPI5_ReadWriteByte(pBuffer[i]);//循环写数
//     // SPI5_ReadWriteByte_DMA(0, pBuffer,NumByteToWrite); //使用DMA写数  
//     W25Qx_Disable();                            //取消片选 
//     W25QXX_Wait_Busy();                       //等待写入结束
//     while(SPI_Status() == 1)
//         ; // 等待DMA传输完成
// } 
// //无检验写SPI FLASH 
// //必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
// //具有自动换页功能 
// //在指定地址开始写入指定长度的数据,但是要确保地址不越界!
// //pBuffer:数据存储区
// //WriteAddr:开始写入的地址(24bit)
// //NumByteToWrite:要写入的字节数(最大65535)
// //CHECK OK
// void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
// {                       
//     uint16_t pageremain;       
//     pageremain=256-WriteAddr%256; //单页剩余的字节数                 
//     if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
//     while(1)
//     {       
//         W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
//         if(NumByteToWrite==pageremain)break;//写入结束了
//          else //NumByteToWrite>pageremain
//         {
//             pBuffer+=pageremain;
//             WriteAddr+=pageremain;    

//             NumByteToWrite-=pageremain;              //减去已经写入了的字节数
//             if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
//             else pageremain=NumByteToWrite;       //不够256个字节了
//         }
//     };        
// } 
// //写SPI FLASH  
// //在指定地址开始写入指定长度的数据
// //该函数带擦除操作!
// //pBuffer:数据存储区
// //WriteAddr:开始写入的地址(24bit)                        
// //NumByteToWrite:要写入的字节数(最大65535)   
// uint8_t W25QXX_BUFFER[4096];         
// void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
// { 
//     uint32_t secpos;
//     uint16_t secoff;
//     uint16_t secremain;       
//      uint16_t i;    
//     uint8_t * W25QXX_BUF;      
//        W25QXX_BUF=W25QXX_BUFFER;         
//      secpos=WriteAddr/4096;//扇区地址  
//     secoff=WriteAddr%4096;//在扇区内的偏移
//     secremain=4096-secoff;//扇区剩余空间大小   
//      //printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
//      if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
//     while(1) 
//     {    
//         W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容
//         for(i=0;i<secremain;i++)//校验数据
//         {
//             if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除        
//         }
//         if(i<secremain)//需要擦除
//         {
//             W25QXX_Erase_Sector(secpos);//擦除这个扇区
//             for(i=0;i<secremain;i++)       //复制
//             {
//                 W25QXX_BUF[i+secoff]=pBuffer[i];      
//             }
//             W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区  

//         }else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间.                    
//         if(NumByteToWrite==secremain)break;//写入结束了
//         else//写入未结束
//         {
//             secpos++;//扇区地址增1
//             secoff=0;//偏移位置为0      

//                pBuffer+=secremain;  //指针偏移
//             WriteAddr+=secremain;//写地址偏移       
//                NumByteToWrite-=secremain;                //字节数递减
//             if(NumByteToWrite>4096)secremain=4096;    //下一个扇区还是写不完
//             else secremain=NumByteToWrite;            //下一个扇区可以写完了
//         }     
//     };     
// }
// //擦除整个芯片          
// //等待时间超长...
// void W25QXX_Erase_Chip(void)   
// {                                   
//     W25QXX_Write_Enable();                  //SET WEL 
//     W25QXX_Wait_Busy();   
//       W25Qx_Enable();                            //使能器件   
//     SPI5_ReadWriteByte(W25X_ChipErase);        //发送片擦除命令  
//     W25Qx_Disable();                            //取消片选               
//     W25QXX_Wait_Busy();                      //等待芯片擦除结束
// }   
// //擦除一个扇区
// //Dst_Addr:扇区地址 根据实际容量设置
// //擦除一个扇区的最少时间:150ms
// void W25QXX_Erase_Sector(uint32_t Dst_Addr)   
// {  
//     //监视falsh擦除情况,测试用   
//      //printf("fe:%x\r\n",Dst_Addr);      
//      Dst_Addr*=4096;
//     W25QXX_Write_Enable();                  //SET WEL      
//     W25QXX_Wait_Busy();   
//       W25Qx_Enable();                            //使能器件   
//     SPI5_ReadWriteByte(W25X_SectorErase);   //发送扇区擦除指令 
//     if(W25QXX_TYPE==W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
//     {
//         SPI5_ReadWriteByte((uint8_t)((Dst_Addr)>>24)); 
//     }
//     SPI5_ReadWriteByte((uint8_t)((Dst_Addr)>>16));  //发送24bit地址    
//     SPI5_ReadWriteByte((uint8_t)((Dst_Addr)>>8));   
//     SPI5_ReadWriteByte((uint8_t)Dst_Addr);  
//     W25Qx_Disable();                            //取消片选               
//     W25QXX_Wait_Busy();                       //等待擦除完成
// }  
// //等待空闲
// void W25QXX_Wait_Busy(void)   
// {   
//     while((W25QXX_ReadSR(1)&0x01)==0x01);   // 等待BUSY位清空
// }  
// //进入掉电模式
// void W25QXX_PowerDown(void)   
// { 
//       W25Qx_Enable();                            //使能器件   
//     SPI5_ReadWriteByte(W25X_PowerDown);     //发送掉电命令  
//     W25Qx_Disable();                            //取消片选               
//     HAL_Delay(3);                            //等待TPD  
// }   

// //唤醒
// void W25QXX_WAKEUP(void)   
// {  
//       W25Qx_Enable();                                //使能器件   
//     SPI5_ReadWriteByte(W25X_ReleasePowerDown);  //  send W25X_PowerDown command 0xAB    
//     W25Qx_Disable();                                //取消片选               
//     HAL_Delay(3);                                //等待TRES1
// } 

#endif // 0
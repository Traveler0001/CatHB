/* 文件： w25flash.h
 * 功能描述： Flash 存储器W25Q128的驱动程序
 * 作者：王维波
 * 修改日期：2019-06-05
 * W25Q128 芯片参数： 16M字节，24位地址线
 * 分为256个Block，每个Block 64K字节
 * 一个Block又分为16个Sector，共4096个Sector，每个Sector 4K字节
 * 一个Sector又分为16个Page，共65536个Page，每个Page 256字节
 * 写数据操作的基本单元是Page，一次连续写入操作不能超过一个Page的范围。写的Page必须是擦除过的。
 */

#ifndef __W25QXX_H
#define __W25QXX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "spi.h" //使用其中的变量 hspi2，表示SPI1接口

#define Fatfs 1

#if Fatfs
#include "fatfs.h"
#include "usart.h"
#endif // !fatfs

#define PRINTF_DEBUG printf
#define W25Q80       0XEF13
#define W25Q16       0XEF14
#define W25Q32       0XEF15
#define W25Q64       0XEF16
#define W25Q128      0XEF17

// W25Q128一共是16M，这里只分出来了12M使用，是为了后续的测试USB读卡器做准备
#define W25Q128FV_SECTOR_SIZE             4096                                                        // 一个扇区是4KiB,即4096Byte
#define W25Q128FV_BLOCKINCLUDESECTOR_BUMB 16                                                          // 一个块含有16个扇区
#define W25Q128FV_BLOCK_SIZE              (W25Q128FV_SECTOR_SIZE * W25Q128FV_BLOCKINCLUDESECTOR_BUMB) // 一个块是4*16=64KiB
#define W25Q128FV_BLOCK_NUMB              256                                                         // W25Q128共有256个块
#define W25Q128FV_SECTOR_NUMB             (W25Q128FV_BLOCK_NUMB * W25Q128FV_BLOCKINCLUDESECTOR_BUMB)
#define W25Q128FV_SIZE                    (W25Q128FV_BLOCK_SIZE * W25Q128FV_BLOCK_NUMB) // W25Q128总容量为256*16*4096= 16,777,216Byte = 16384KiB = 16MiB
#define W25Q128FV_PAGE_SIZE               256                                           // 单次写入最多字节数

/**
 * @brief  W25Q128FV Commands
 */
#define W25X_WriteEnable      0x06
#define W25X_WriteDisable     0x04
#define W25X_ReadStatusReg    0x05
#define W25X_WriteStatusReg   0x01
#define W25X_ReadData         0x03
#define W25X_FastReadData     0x0B
#define W25X_FastReadDual     0x3B
#define W25X_PageProgram      0x02
#define W25X_BlockErase       0xD8
#define W25X_SectorErase      0x20
#define W25X_ChipErase        0xC7
#define W25X_PowerDown        0xB9
#define W25X_ReleasePowerDown 0xAB
#define W25X_DeviceID         0xAB

#define W25X_ManufactDeviceID 0x90
#define W25X_JedecDeviceID    0x9F

// 这个地方的片选信号，是最后一个自定义的片选IO的引脚
#define W25Qx_Enable()  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET)
#define W25Qx_Disable() HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET)

uint16_t BSP_W25QXX_Init(void);

uint16_t BSP_W25QXX_ReadID(void); // 读取FLASH ID
uint8_t BSP_W25QXX_ReadSR(void);  // 读取状态寄存器

void BSP_W25QXX_Write_SR(uint8_t sr); // 写状态寄存器
void BSP_W25QXX_Write_Enable(void);   // 写使能
void BSP_W25QXX_Write_Disable(void);  // 写保护

void BSP_W25QXX_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead); // 读取flash
void BSP_W25QXX_Write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void BSP_W25QXX_Write(const uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite); // 写入flash

void BSP_W25QXX_Erase_Chip(void);                // 整片擦除
void BSP_W25QXX_Erase_Sector(uint32_t Dst_Addr); // 扇区擦除

void BSP_W25QXX_Wait_Busy(void); // 等待空闲
void BSP_W25QXX_PowerDown(void); // 进入掉电模式

void BSP_W25QXX_WAKEUP(void); // 唤醒
void BSP_W25QXX_Test(void);

#if Fatfs
void fatfs_test(void);
void lv_fatfs_test(void);
#endif // !fatfs

#ifdef __cplusplus
}
#endif

#endif /* __W25QXX_H */

// #ifndef __W25QXX_H
// #define __W25QXX_H

// #include "main.h"
    
// //W25X系列/Q系列芯片列表       
// #define W25Q80     0XEF13     
// #define W25Q16     0XEF14
// #define W25Q32     0XEF15
// #define W25Q64     0XEF16
// #define W25Q128    0XEF17
// #define W25Q256 0XEF18

// extern uint16_t W25QXX_TYPE;                    //定义W25QXX芯片型号           

// // #define    W25QXX_CS         PFout(6)          //W25QXX的片选信号
// #define W25Qx_Enable()  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET)
// #define W25Qx_Disable() HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET)

// // 
// //指令表
// #define W25X_WriteEnable        0x06 
// #define W25X_WriteDisable        0x04 
// #define W25X_ReadStatusReg1        0x05 
// #define W25X_ReadStatusReg2        0x35 
// #define W25X_ReadStatusReg3        0x15 
// #define W25X_WriteStatusReg1    0x01 
// #define W25X_WriteStatusReg2    0x31 
// #define W25X_WriteStatusReg3    0x11 
// #define W25X_ReadData            0x03 
// #define W25X_FastReadData        0x0B 
// #define W25X_FastReadDual        0x3B 
// #define W25X_PageProgram        0x02 
// #define W25X_BlockErase            0xD8 
// #define W25X_SectorErase        0x20 
// #define W25X_ChipErase            0xC7 
// #define W25X_PowerDown            0xB9 
// #define W25X_ReleasePowerDown    0xAB 
// #define W25X_DeviceID            0xAB 
// #define W25X_ManufactDeviceID    0x90 
// #define W25X_JedecDeviceID        0x9F 
// #define W25X_Enable4ByteAddr    0xB7
// #define W25X_Exit4ByteAddr      0xE9

// void W25QXX_Init(void);
// uint8_t SPI_Status(void);
// uint16_t  W25QXX_ReadID(void);                  //读取FLASH ID
// uint8_t W25QXX_ReadSR(uint8_t regno);             //读取状态寄存器 
// void W25QXX_4ByteAddr_Enable(void);     //使能4字节地址模式
// void W25QXX_Write_SR(uint8_t regno,uint8_t sr);   //写状态寄存器
// void W25QXX_Write_Enable(void);          //写使能 
// void W25QXX_Write_Disable(void);        //写保护
// void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
// void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);   //读取flash
// void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);//写入flash
// void W25QXX_Erase_Chip(void);              //整片擦除
// void W25QXX_Erase_Sector(uint32_t Dst_Addr);    //扇区擦除
// void W25QXX_Wait_Busy(void);               //等待空闲
// void W25QXX_PowerDown(void);            //进入掉电模式
// void W25QXX_WAKEUP(void);                //唤醒

// #endif
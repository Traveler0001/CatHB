// #include "hlw8032.h"
// // #include "usart.h"
// // #include "dma.h"
// #include "stm32f4xx_hal.h"
// #include "stdio.h"
// #include "math.h"

#if 0

#define voltageCoefficient (118 * 1000 * 4) / (1000 * 1000) // 电压系数
#define currentCoefficient (1 / (0.001 * 1000))				// 电流系数
#define TIMEOUTNUM 200

#define FREERTOS 1

#if FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif
HLW8032_StateReg hlwState;
HLW8032_STATUS HLW8032_status;

static void hlw8032_structClear()
{
	for (uint8_t i = 0; i < 26; i++)
	{
		hlwState.rxData[i] = 0;
	}
	hlwState.stateRegNone = 0;				  // 状态寄存器保留4位
	hlwState.stateRegVoltage = 0;			  // 电压状态寄存器
	hlwState.stateRegCurrent = 0;			  // 电流状态寄存器
	hlwState.stateRegPower = 0;				  // 功率状态寄存器
	hlwState.stateRegVoltageCurrentPower = 0; // 电压电流功率状态寄存器
	/** 状态寄存器原始值
	 * 1、 当 State REG = 0xaa 时，芯片误差修正功能失效，此时电压参数寄存器、电流参数寄存器和功率参数寄存器不可用;
	 * 2、 当 State REG = 0x55 时，芯片误差修正功能正常，此时电压参数寄存器、电流参数寄存器和功率参数寄存器可用,且电压寄存器、电流寄存器和功率寄存器未溢出;
	 * 3、 当 State REG = 0xfx 时(x 为 0-f 的任意数)，芯片误差修正功能正常, 此时电压参数寄存器、电流参数寄存器和功率参数寄存器可用, State REG 的相应位为 1 时表示相应的寄存器溢出，溢出表示电流、电压或功率值非常小，接近 0;
	 */
	hlwState.stateReg = 0;
	hlwState.checkReg = 0;				   // 检测寄存器
	hlwState.voltagePrameterReg = 0;	   // 电压值参数寄存器
	hlwState.voltageReg = 0;			   // 电压寄存器
	hlwState.currentPrameterReg = 0;	   // 电流值参数寄存器
	hlwState.currentReg = 0;			   // 电流寄存器
	hlwState.powerPrameterReg = 0;		   // 功率值参数寄存器
	hlwState.powerReg = 0;				   // 功率寄存器
	hlwState.dataUpdataRegPFCarrySign = 0; // PF寄存器进位标志位
	hlwState.dataUpdataRegVoltage = 0;	   // 电压寄存器更新标志位
	hlwState.dataUpdataRegCurrent = 0;	   // 电流寄存器更新标志位
	hlwState.dataUpdataRegPower = 0;	   // 功率寄存器更新标志位
	hlwState.dataUpdataRegNone = 0;		   // 保留4位
	hlwState.dataUpdataReg = 0;			   // 数据更新寄存器
	hlwState.pfReg = 0;					   // PF寄存器
	hlwState.checkSumReg = 0;			   // 校验和寄存器
}

static void hlw8032_structUpdata()
{
	hlwState.stateReg = hlwState.rxData[0];
	hlwState.stateRegNone = 0xF;
	hlwState.stateRegVoltage = hlwState.stateReg & 0x08;
	hlwState.stateRegCurrent = hlwState.stateReg & 0x04;
	hlwState.stateRegPower = hlwState.stateReg & 0x02;
	hlwState.stateRegVoltageCurrentPower = hlwState.stateReg & 0x01;
	hlwState.checkReg = hlwState.rxData[1];
	hlwState.voltagePrameterReg = (hlwState.rxData[2] << 16) | (hlwState.rxData[3] << 8) | hlwState.rxData[4];
	hlwState.voltageReg = (hlwState.rxData[5] << 16) | (hlwState.rxData[6] << 8) | hlwState.rxData[7];
	hlwState.currentPrameterReg = (hlwState.rxData[8] << 16) | (hlwState.rxData[9] << 8) | hlwState.rxData[10];
	hlwState.currentReg = (hlwState.rxData[11] << 16) | (hlwState.rxData[12] << 8) | hlwState.rxData[13];
	hlwState.powerPrameterReg = (hlwState.rxData[14] << 16) | (hlwState.rxData[15] << 8) | hlwState.rxData[16];
	hlwState.powerReg = (hlwState.rxData[17] << 16) | (hlwState.rxData[18] << 8) | hlwState.rxData[19];
	hlwState.dataUpdataReg = hlwState.rxData[20];
	hlwState.dataUpdataRegPFCarrySign = hlwState.dataUpdataReg & 0x80;
	hlwState.dataUpdataRegVoltage = hlwState.dataUpdataReg & 0x40;
	hlwState.dataUpdataRegCurrent = hlwState.dataUpdataReg & 0x20;
	hlwState.dataUpdataRegPower = hlwState.dataUpdataReg & 0x10;
	hlwState.pfReg = (hlwState.rxData[21] << 8) | hlwState.rxData[22];
	hlwState.checkSumReg = hlwState.rxData[23];
	hlwState.voltageValue = (float)hlwState.voltagePrameterReg / hlwState.voltageReg * voltageCoefficient;
	hlwState.currentValue = (float)hlwState.currentPrameterReg / hlwState.currentReg * currentCoefficient;
	hlwState.activePowerValue = (float)hlwState.powerPrameterReg / hlwState.powerReg * voltageCoefficient * currentCoefficient;
	hlwState.apparentPowerValue = (float)hlwState.voltageValue * hlwState.currentValue;
	hlwState.factor = hlwState.activePowerValue / hlwState.apparentPowerValue;
}

static HLW8032_STATUS hlw8032_timeoutcheck(uint32_t *starttime, uint32_t timeout)
{
	uint8_t status;
	uint32_t maxtime = 0xFFFFFFFF;

#if FREERTOS
	uint32_t nowtime = xTaskGetTickCount();
#else
	uint32_t nowtime = HAL_GetTick();
#endif

	if (*starttime > nowtime)
		status = ((maxtime - *starttime + nowtime) > timeout) ? HLW8032RECTIMEOUT : HLW8032OK;
	else
		status = ((nowtime - *starttime) > timeout) ? HLW8032RECTIMEOUT : HLW8032OK;
	return status;
}

static HLW8032_STATUS hlw8032_Read()
{
	// 等待空闲中断->DMA接收->等待空闲中断

	uint32_t timeout = 0; // 超时计数器
	hlw8032_structClear();
	__HAL_UART_CLEAR_IDLEFLAG(&huart2); // Clear USART2 IDLE flag

#if FREERTOS
	timeout = xTaskGetTickCount();
	// 等待空闲中断
	while (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) == 0x00u)
	{
		if (hlw8032_timeoutcheck(&timeout, TIMEOUTNUM))
			return HLW8032RECTIMEOUT; // timeout
		else
			vTaskDelay(0);
	}
	
	__HAL_UART_CLEAR_IDLEFLAG(&huart2); // Clear USART2 IDLE flag
	HAL_UART_Receive_DMA(&huart2, hlwState.rxData, 24);
	timeout = xTaskGetTickCount();
	// 清除中断标志
	// 等待再次发生空闲中断
	while (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) == 0x00u)
		if (hlw8032_timeoutcheck(&timeout, TIMEOUTNUM))
			return HLW8032RECTIMEOUT; // timeout
		else
			vTaskDelay(0);
	timeout = 0;
#else
	timeout = HAL_GetTick();
	while (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) == 0x00u)
		if ((HAL_GetTick() - timeout) >= TIMEOUTNUM)
			return HLW8032RECTIMEOUT; // timeout
		else
			HAL_Delay(1);
	// 清除中断标志
	__HAL_UART_CLEAR_IDLEFLAG(&huart2); // Clear USART2 IDLE flag
	// hlw8032发送数据需要55ms,间隔50ms,接收两次,从中找出一次完整的数据
	HAL_UART_Receive_DMA(&huart2, rxBuffer, 24);
	timeout = HAL_GetTick();
	while (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) == 0x00u)
		if ((HAL_GetTick() - timeout) >= TIMEOUTNUM)
			return HLW8032RECTIMEOUT; // timeout
		else
			HAL_Delay(1);
#endif
	HAL_UART_DMAStop(&huart2); // Stop DMA reception
	return HLW8032OK;
}

/**
 * @brief 读取HLW8032寄存器的值
 *
 * @return uint8_t 0:读取数据正常; 1:芯片寄存器不可用; 2:开路状态; 3:错误,未在缓冲区中找到数据帧; 4:帧校验失败
 */
static HLW8032_STATUS hlw8032_RDataProcess(void)
{
	uint16_t checkSum = 0;

	HLW8032_status = hlw8032_Read();

	if (HLW8032_status == HLW8032OK)
	{
		for (uint8_t i = 0; i < 24; i++)
			if (hlwState.rxData[i] == 0x55 && hlwState.rxData[i + 1] == 0x5A)
			{
				i = 24;
				continue; // 正常状态
			}
			else if (hlwState.rxData[i] == 0xAA && hlwState.rxData[i + 1] == 0x5A)
				return HLW8032REGERR; // 芯片寄存器不可用
			else if (((hlwState.rxData[i] & 0xF0) == 0xF0) && hlwState.rxData[i + 1] == 0x5A)
				return HLW8032ACOPEN; // 开路状态
			else if (i > 23)
				return HLW8032NODATA; // 错误,未在缓冲区中找到数据帧

		// 数据帧校验计算
		for (uint8_t i = 2; i < 23; i++)
			checkSum += hlwState.rxData[i];
		checkSum &= 0x00FF;
		hlwState.rxData[24] = (uint8_t)checkSum;
		checkSum = 0;

		// 判断当前帧接收是否正确
		if (hlwState.rxData[24] == hlwState.rxData[23])
			hlwState.rxData[25] = 1;
		else
		{
			hlwState.rxData[25] = 0;
			return HLW8032DATAERR; // 帧校验失败
		}

		hlw8032_structUpdata();

		return HLW8032OK; // 数据接收完成
	}
	else
		return HLW8032_status;
}

void hle8032_ReadPF(void)
{
}

uint8_t hlw8032statusInfo[hlw8032statusInfoMax] = {0};

HLW8032_STATUS hlw8032_DataRead(void)
{
	HLW8032_status = hlw8032_RDataProcess();
	switch (HLW8032_status) // Read data from HLW8032
	{
	case HLW8032OK:
		sprintf(hlw8032statusInfo, "DATA OK");
		break;
	case HLW8032REGERR:
		sprintf(hlw8032statusInfo, "CHIP REG CAN'T USED");
		break;
	case HLW8032ACOPEN:
		sprintf(hlw8032statusInfo, "AC IN is open");
		break;
	case HLW8032NODATA:
		sprintf(hlw8032statusInfo, "CAN'T FIND DATA IN RX BUF");
		break;
	case HLW8032DATAERR:
		sprintf(hlw8032statusInfo, "DATA CHECK ERROR");
		break;
	case HLW8032RECTIMEOUT:
		sprintf(hlw8032statusInfo, "DATA RECEIVE TOMEOUT");
	}
#if showOfUsart
	HAL_UART_Transmit(&huart1, statusInfoMax, sizeof(statusInfoMax), HAL_MAX_DELAY);
#endif
	return HLW8032_status;
}

#endif
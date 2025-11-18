#ifndef __HLW8032_H
#define __HLW8032_H

#include "main.h"

#define hlw8032statusInfoMax 30
extern uint8_t hlw8032statusInfo[hlw8032statusInfoMax];

typedef enum
{
    HLW8032OK,          // HLW8032正常
    HLW8032REGERR,      // 芯片寄存器无法使用
    HLW8032ACOPEN,      // 交流输入开路
    HLW8032NODATA,      // 数据包内无有效数据
    HLW8032DATAERR,     // 数据校验错误
    HLW8032RECTIMEOUT,  // 接收数据超时
    HLW8032NULL         // 初始化状态
} HLW8032_STATUS;

typedef struct
{
    uint8_t rxData[26];                      // 0-23: HLW8032寄存器数据; 24:计算出的检验值; 25:寄存器校验值与计算校验值对比结果,1为校验成功,0为校验失败
    uint8_t stateRegNone : 4;                // 状态寄存器保留4位
    uint8_t stateRegVoltage : 1;             // 电压状态寄存器
    uint8_t stateRegCurrent : 1;             // 电流状态寄存器
    uint8_t stateRegPower : 1;               // 功率状态寄存器
    uint8_t stateRegVoltageCurrentPower : 1; // 电压电流功率状态寄存器
    /** 状态寄存器原始值
     * 1、 当 State REG = 0xaa 时，芯片误差修正功能失效，此时电压参数寄存器、电流参数寄存器和功率参数寄存器不可用;
     * 2、 当 State REG = 0x55 时，芯片误差修正功能正常，此时电压参数寄存器、电流参数寄存器和功率参数寄存器可用,且电压寄存器、电流寄存器和功率寄存器未溢出;
     * 3、 当 State REG = 0xfx 时(x 为 0-f 的任意数)，芯片误差修正功能正常, 此时电压参数寄存器、电流参数寄存器和功率参数寄存器可用, State REG 的相应位为 1 时表示相应的寄存器溢出，溢出表示电流、电压或功率值非常小，接近 0;
     */
    uint8_t stateReg;
    uint8_t checkReg;                     // 检测寄存器
    uint32_t voltagePrameterReg;          // 电压值参数寄存器
    uint32_t voltageReg;                  // 电压寄存器
    uint32_t currentPrameterReg;          // 电流值参数寄存器
    uint32_t currentReg;                  // 电流寄存器
    uint32_t powerPrameterReg;            // 功率值参数寄存器
    uint32_t powerReg;                    // 功率寄存器
    uint8_t dataUpdataRegPFCarrySign : 1; // PF寄存器进位标志位
    uint8_t dataUpdataRegVoltage : 1;     // 电压寄存器更新标志位
    uint8_t dataUpdataRegCurrent : 1;     // 电流寄存器更新标志位
    uint8_t dataUpdataRegPower : 1;       // 功率寄存器更新标志位
    uint8_t dataUpdataRegNone : 4;        // 保留4位
    uint8_t dataUpdataReg;                // 数据更新寄存器
    uint8_t pfReg;                        // PF寄存器
    uint8_t checkSumReg;                  // 校验和寄存器
    float voltageValue;                   // 电压值
    float currentValue;                   // 电流值
    float activePowerValue;               // 有功功率
    float apparentPowerValue;             // 视在功率
    float factor;                         // 功率因数
} HLW8032_StateReg;

extern HLW8032_StateReg hlwState;
extern HLW8032_STATUS HLW8032_status;

void hle8032_ReadPF(void);
HLW8032_STATUS hlw8032_DataRead(void);

#endif // !__HLW8032_H
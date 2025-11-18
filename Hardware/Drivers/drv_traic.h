#ifndef __DRV_TRAIC_H__
#define __DRV_TRAIC_H__

#include "stdint.h"
#include "stdbool.h"
#include "math.h"
#include "gpio.h"
#include <stdint.h>
#include <sys/_intsup.h>

// 控制阶段枚举
typedef enum {
    CTR_STAGE_FAST_HEAT,    // 快速升温阶段（温差＞40℃）
    CTR_STAGE_HOLD_RECORD,  // 维持并记录阶段（温差≤40℃）
    CTR_STAGE_STABLE_CHECK, // 稳定性判定阶段
    CTR_STAGE_FINE_TUNE     // 精细调节阶段
} CtrlStageTypeDef;

// 温度控制配置参数结构体
typedef struct {
    uint32_t control_period; // 控制周期(ms)，默认2000ms
    float max_temp_limit;    // 最高温度限制(℃)
    float min_temp_limit;    // 最低温度限制(℃)
    float final_stable_delta;// 最终稳定温差（0.4℃）
    float overshoot_threshold;  // 过冲判定阈值（如+1℃，即超过目标1℃判定为过冲）
    float temp_diff_segment[6]; // 温差区间划分点（示例：0,5,10,40,∞）
    uint16_t power_pct[6];              // 温差对应功率百分比
    uint16_t temp_segment[8]; // 设定温度区间划分点（示例：0,50,100,170,220,250,260
    uint16_t temp_pct[8]; // 示例：{0.05,0.07,0.90,0.15,0.20,0.25,0.30}
} temp_profile_t;

// 温度控制动态数据结构体
typedef struct {
    float temp_set;          // 目标设定温度(℃)
    float temp_current;      // 当前实测温度(℃)
    float temp_history[4];   // 温度记录缓存（4次）
    uint8_t record_cnt;      // 已记录次数（0~3）
    float temp_stddev;       // 温度标准差
    uint16_t current_on_ms;  // 当前导通时间(ms)
} TempCtrTypeDef;

// 总控制结构体
typedef struct {
    temp_profile_t tempProfile;      // 配置参数
    TempCtrTypeDef tempCtr;          // 动态数据
    // 硬件状态（示例）
    struct {
        bool TRAICENSTATUS;  // 加热使能
        bool TRAICRUNSTATUS; // 加热运行状态
    } traicStatus;
    struct {
        bool FANRUNSTATUS;   // 风扇运行状态
    } fanStatus;
} HTRAIcTypeDef;

// 全局控制实例
extern HTRAIcTypeDef htraic;

// 函数声明
void TRAIC_ParamInit(void);
void TRAIC_CalcDynamicPower(void);
void TRAIC_ExecuteHeating(void);
void taskTempCtr(void *argument);
void TRAIC_SetTemp(float new_temp);

#endif // __DRV_TRAIC_H__
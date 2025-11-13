#ifndef __DRV_TRAIC_H__
#define __DRV_TRAIC_H__

// /* -------------------------- 全局状态与类型定义 -------------------------- */
// 温度状态枚举
#include <stdbool.h>
#include <stdint.h>
typedef enum {
    TEMP_ERROR,     // 温度采集异常
    TEMPSETERR,     // 温度设定错误
    LOWOFSET,         // 低于目标温度1℃
    ARRIVEDSET,       // 在目标温度±1℃内
    HIGHSET           // 高于目标温度1℃
} temp_status_e;

// 加热/风扇状态
typedef struct {
    bool TRAICENSTATUS;     // 加热使能状态
    bool TRAICRUNSTATUS;    // 加热运行状态
} traic_status_e;

typedef struct {
    bool FANENSTATUS;
    bool FANRUNSTATUS;
} fan_status_e;

typedef enum{
    ZEROSPEED,
    FULLSPEED,
    HALFSPEED,
    LOWSPEED,
    KEEPSPEED
} heating_speed;

// // 加热阶段枚举（新增启动阶段）
// typedef enum {
//     HEATING_INIT,       // 首次启动全功率加热
//     HEATING_WAIT_STABLE,// 等待温度稳定
//     HEATING_REGULAR     // 常规调功阶段
// } heating_phase_e;

// 温度控制全局结构体（与其他任务共享）
typedef struct {
    float temp_set;              // 目标温度(℃)
} temp_ctr_t;

/* -------------------------- 调功参数配置（集中管理） -------------------------- */
/**
 * 温度段与调功参数配置结构体
 * 可根据硬件特性调整所有阈值、周期和占空比参数
 */
typedef struct {
    // 加热速率阈值
    float fullSpeedThreshold;           // 全速段阈值
    float halfSpeedThreshold;           // 半速段阈值
    float lowSpeedThreshold;            // 低速段阈值
    float keepSpeedThreshold;           // 维持段阈值

    // 各阶段调功周期(ms)
    uint32_t period_full;           // 全速段周期
    uint32_t period_half;           // 半速段周期
    uint32_t period_low ;           // 低速段周期
    uint32_t period_keep;           // 维持段周期
    
    // 各阶段占空比参数(%)
    uint8_t duty_full;      // 低温段占空比（如80%）
    uint8_t duty_half;      // 中温段占空比（如50%）
    uint8_t duty_low ;       // 高温段占空比（如30%）
    uint8_t duty_keep;      // 维持段占空比
    
    // 安全参数
    float max_temp_limit;        // 加热器高极限温度（230℃）
    float min_temp_limit;       // 加热器低极限温度（230℃）
} temp_profile_t;

/**
 * 全局调功参数配置
 * 所有参数可根据实测结果调整，无需修改逻辑代码
 */
static const temp_profile_t temp_profile = {
    // 加热速率阈值
    .fullSpeedThreshold = 100,           // 全速段阈值
    .halfSpeedThreshold = 50,          // 半速段阈值
    .lowSpeedThreshold  = 10,            // 低速段阈值
    .keepSpeedThreshold = 0.5,           // 维持阶段阈值
    
    // 调功周期
    .period_full =  1000,
    .period_half =  2000,
    .period_low  =  3000, 
    .period_keep =  2000,
    
    // 占空比参数
    .duty_full  = 100,
    .duty_half  = 50,
    .duty_low   = 10,
    .duty_keep  = 5,
    
    // 安全参数
    .max_temp_limit = 230.0f,
    .min_temp_limit = -20.0f
};


// /* -------------------------- 调功参数计算模块（抽离函数） -------------------------- */
// /**
//  * 调功参数计算结果
//  */
// typedef struct {
//     uint32_t period;           // 调功周期(ms)
//     uint8_t duty;              // 占空比(%)
//     bool is_overlimit;         // 是否超过极限温度
// } pwm_param_t;

typedef struct{
    traic_status_e  traicStatus;
    fan_status_e    fanStatus;
    temp_profile_t *tempProfile;
    temp_ctr_t tempCtr;
    temp_status_e tempStatus;
    heating_speed heatSpeed;
    
} TRAIC_HandleTypeDef;

extern TRAIC_HandleTypeDef htraic;

#endif // DRV_TRAIC_H__
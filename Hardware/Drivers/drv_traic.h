// #ifndef __DRV_TRAIC_H__
// #define __DRV_TRAIC_H__

// #include "cmsis_os.h"
// #include "hardware_list.h"
// #include "lvgl.h"
// #include "gpio.h"
// #include <stdint.h>
// #include <stdbool.h>

// /* -------------------------- 全局状态与类型定义 -------------------------- */
// // 温度状态枚举
// typedef enum {
//     TEMP_ERROR = -1,  // 温度采集异常
//     LOWOFSET,         // 低于目标温度1℃
//     ARRIVEDSET,       // 在目标温度±1℃内
//     HIGHSET           // 高于目标温度1℃
// } temp_status_e;

// // 加热/风扇状态枚举
// typedef enum {
//     TRAICSTOP = 0,
//     TRAICRUN
// } traic_status_e;

// typedef enum {
//     FANSTOP = 0,
//     FANRUN
// } fan_status_e;

// // 加热阶段枚举（新增启动阶段）
// typedef enum {
//     HEATING_INIT,       // 首次启动全功率加热
//     HEATING_WAIT_STABLE,// 等待温度稳定
//     HEATING_REGULAR     // 常规调功阶段
// } heating_phase_e;

// // 温度控制全局结构体（与其他任务共享）
// typedef struct {
//     float temp_set;              // 目标温度(℃)
//     temp_status_e tempStatus;    // 温度状态
//     traic_status_e traicEnStatus;// 加热使能状态
//     traic_status_e traicRunStatus;// 加热运行状态
//     fan_status_e fanEnStatus;    // 风扇使能状态
//     fan_status_e fanRunStatus;   // 风扇运行状态
// } temp_ctr_t;
// temp_ctr_t temp_ctr = {0};      // 全局温控变量

// // MAX31865相关全局状态
// typedef struct {
//     uint8_t enable;              // 使能状态（MAX31865ENABLE/MAX31865DISABLE）
//     uint8_t status;              // 状态码（MAX31865OK/错误码）
//     struct {
//         float temperature;       // 温度值(℃)
//     } rtdData;
// } max31865_t;
// max31865_t hmax31865 = {0};

// typedef struct{
//     traic_status_e traicEnStatus;
// } TRAIC_HandleTypeDef;

// TRAIC_HandleTypeDef htraic;

// // LVGL界面全局变量（由GUI Guider生成）
// extern lv_ui guider_ui;

// // 互斥锁（线程安全）
// osMutexId_t mutexTempCtrHandle;
// const osMutexAttr_t mutexTempCtrAttr = {.name = "mutexTempCtr"};
// osMutexId_t mutexLvglHandle;     // 假设已在其他地方初始化


// /* -------------------------- 调功参数配置（集中管理） -------------------------- */
// /**
//  * 温度段与调功参数配置结构体
//  * 可根据硬件特性调整所有阈值、周期和占空比参数
//  */
// typedef struct {
//     // 温度段划分阈值(℃)
//     float low_threshold;         // 低温段上限（如80℃）
//     float high_threshold;        // 高温段下限（如180℃）
//     float small_delta;           // 小温差阈值（如5℃）
    
//     // 各阶段调功周期(ms)
//     uint32_t period_low;         // 低温段周期（如1000ms）
//     uint32_t period_mid;         // 中温段周期（如5000ms）
//     uint32_t period_high;        // 高温段周期（如10000ms）
    
//     // 各阶段占空比参数(%)
//     uint8_t duty_low_base;       // 低温段基础占空比（如80%）
//     uint8_t duty_mid_fixed;      // 中温段固定占空比（如50%）
//     uint8_t duty_high_max;       // 高温段最大占空比（如30%）
    
//     // 启动阶段参数
//     float init_stop_delta;       // 启动阶段停止温差（30℃）
//     uint32_t stable_delay;       // 稳定等待时间（如5000ms）
//     float stable_threshold;      // 温度稳定波动阈值（如1.0℃）
    
//     // 安全参数
//     float max_temp_limit;        // 加热器极限温度（230℃）
// } temp_profile_t;

// /**
//  * 全局调功参数配置
//  * 所有参数可根据实测结果调整，无需修改逻辑代码
//  */
// static const temp_profile_t temp_profile = {
//     // 温度段阈值
//     .low_threshold = 80.0f,
//     .high_threshold = 180.0f,
//     .small_delta = 5.0f,
    
//     // 调功周期
//     .period_low = 1000,
//     .period_mid = 5000,
//     .period_high = 10000,
    
//     // 占空比参数
//     .duty_low_base = 80,
//     .duty_mid_fixed = 50,
//     .duty_high_max = 30,
    
//     // 启动阶段参数
//     .init_stop_delta = 30.0f,    // 温差≤30℃时停止启动阶段
//     .stable_delay = 5000,        // 等待5秒确认稳定
//     .stable_threshold = 1.0f,    // 波动≤1℃视为稳定
    
//     // 安全参数
//     .max_temp_limit = 230.0f
// };


// /* -------------------------- 调功参数计算模块（抽离函数） -------------------------- */
// /**
//  * 调功参数计算结果
//  */
// typedef struct {
//     uint32_t period;           // 调功周期(ms)
//     uint8_t duty;              // 占空比(%)
//     bool is_overlimit;         // 是否超过极限温度
// } pwm_param_t;

// /**
//  * 常规调功阶段参数计算（低温/中温/高温段）
//  * @param T_current: 当前温度(℃)
//  * @param T_set: 目标温度(℃)
//  * @return 计算后的调功参数
//  */
// static pwm_param_t calc_regular_pwm(float T_current, float T_set)
// {
//     pwm_param_t param = {0};
//     float delta_T = T_set - T_current;

//     // 低温段：短周期+动态高占空比
//     if (T_current < temp_profile.low_threshold && delta_T > temp_profile.small_delta) {
//         param.period = temp_profile.period_low;
//         param.duty = temp_profile.duty_low_base + (uint8_t)(delta_T * 2);
//         param.duty = (param.duty > 100) ? 100 : param.duty;
//     }
//     // 中温段：长周期+固定占空比
//     else if (T_current >= temp_profile.low_threshold && T_current < temp_profile.high_threshold) {
//         param.period = temp_profile.period_mid;
//         param.duty = temp_profile.duty_mid_fixed;
//     }
//     // 高温段：更长周期+低占空比
//     else {
//         param.period = temp_profile.period_high;
//         param.duty = 20 + (uint8_t)(delta_T * 2);
//         param.duty = (param.duty > temp_profile.duty_high_max) ? temp_profile.duty_high_max : param.duty;
//         param.duty = (param.duty < 20) ? 20 : param.duty;
//     }

//     return param;
// }

// /**
//  * 判断温度是否稳定（连续一段时间内波动小于阈值）
//  * @param T_current: 当前温度(℃)
//  * @return true-温度稳定
//  */
// static bool is_temp_stable(float T_current)
// {
//     static float temp_history[5];  // 存储最近5次温度（100ms周期→500ms历史）
//     static uint8_t hist_idx = 0;

//     // 填充历史温度数组
//     temp_history[hist_idx] = T_current;
//     hist_idx = (hist_idx + 1) % (sizeof(temp_history)/sizeof(temp_history[0]));

//     // 计算最大温差
//     float max = temp_history[0], min = temp_history[0];
//     for (uint8_t i = 1; i < (sizeof(temp_history)/sizeof(temp_history[0])); i++) {
//         max = (temp_history[i] > max) ? temp_history[i] : max;
//         min = (temp_history[i] < min) ? temp_history[i] : min;
//     }

//     return (max - min) <= temp_profile.stable_threshold;
// }

// /**
//  * 计算加热阶段与调功参数（整合启动/稳定/常规阶段）
//  * @param T_current: 当前温度(℃)
//  * @param T_set: 目标温度(℃)
//  * @param phase: 当前加热阶段（输入输出）
//  * @param stable_timer: 稳定等待计时器(ms)
//  * @return 调功参数
//  */
// static pwm_param_t calc_heating_param(float T_current, float T_set, 
//                                      heating_phase_e *phase, 
//                                      uint32_t *stable_timer)
// {
//     pwm_param_t param = {0};
//     float delta_T = T_set - T_current;

//     // 极限温度保护（任何阶段都生效）
//     param.is_overlimit = (T_current >= temp_profile.max_temp_limit);
//     if (param.is_overlimit) {
//         *phase = HEATING_REGULAR;  // 强制进入常规阶段（关闭加热）
//         return param;
//     }

//     // 根据当前阶段计算参数
//     switch (*phase) {
//         case HEATING_INIT:
//             // 启动阶段：全功率加热，直到温差≤30℃
//             param.period = 1000;  // 1秒周期
//             param.duty = 100;     // 全功率
//             if (delta_T <= temp_profile.init_stop_delta) {
//                 *phase = HEATING_WAIT_STABLE;  // 进入稳定阶段
//                 *stable_timer = 0;             // 重置计时器
//             }
//             break;

//         case HEATING_WAIT_STABLE:
//             // 稳定阶段：停止加热，等待温度稳定
//             param.period = 1000;
//             param.duty = 0;       // 停止加热
//             *stable_timer += 100; // 累加100ms（任务周期）

//             // 满足等待时间且温度稳定→进入常规阶段
//             if (*stable_timer >= temp_profile.stable_delay && is_temp_stable(T_current)) {
//                 *phase = HEATING_REGULAR;
//             }
//             break;

//         case HEATING_REGULAR:
//             // 常规阶段：调用常规调功计算
//             param = calc_regular_pwm(T_current, T_set);
//             break;
//     }

//     return param;
// }

// /**
//  * 判断当前是否为加热阶段（基于周期和占空比）
//  * @param param: 调功参数
//  * @param pwm_cnt: 周期内计数器(ms)
//  * @return true-加热阶段，false-关闭阶段
//  */
// static bool is_heating_phase(const pwm_param_t *param, uint32_t pwm_cnt)
// {
//     uint32_t on_time = (param->period * param->duty) / 100;  // 加热时间(ms)
//     return (pwm_cnt < on_time);
// }


// /* -------------------------- 辅助函数（指示灯/硬件控制） -------------------------- */
// /**
//  * 更新加热指示灯状态
//  * @param last_traic_run: 上一次加热状态指针
//  */
// static void ChangeLed_traic(traic_status_e *last_traic_run)
// {
//     if (*last_traic_run != temp_ctr.traicRunStatus) {
//         *last_traic_run = temp_ctr.traicRunStatus;
//         const uint32_t retry = 3;
//         uint32_t try_cnt = retry;
//         while (try_cnt-- > 0 && osMutexWait(mutexLvglHandle, 50) != osOK);
//         if (try_cnt >= 0) {
//             if (*last_traic_run == TRAICRUN) {
//                 lv_led_on(guider_ui.Hot_Board_led_traic);
//             } else {
//                 lv_led_off(guider_ui.Hot_Board_led_traic);
//             }
//             osMutexRelease(mutexLvglHandle);
//         }
//     }
// }

// /**
//  * 处理任务间信号（如硬件初始化）
//  */
// static void task_signal_process(void)
// {
//     // 示例：处理TRAIC初始化信号（根据实际需求扩展）
//     // if (task_signal.need_traic_init) {
//     //     TRAIC_App_Init();
//     //     task_signal.need_traic_init = 0;
//     // }
// }


// /* -------------------------- 主任务逻辑（StartTask04） -------------------------- */
// void StartTask04(void *argument)
// {
//     // 初始化互斥锁（若未在其他地方初始化）
//     if (mutexTempCtrHandle == NULL) {
//         mutexTempCtrHandle = osMutexNew(&mutexTempCtrAttr);
//     }

//     // 任务状态变量
//     traic_status_e last_traic_run = TRAICSTOP;
//     fan_status_e last_fan_run = FANSTOP;
//     uint32_t traic_off_delay = 0;       // 加热关闭延迟计数器
//     const uint32_t MIN_OFF_TIME = 5;    // 最小关闭时间（5个任务周期→500ms）

//     // 调功周期与计数器
//     uint32_t pwm_period = temp_profile.period_low;
//     uint32_t pwm_cnt = 0;

//     // 启动阶段管理变量
//     heating_phase_e heating_phase = HEATING_INIT;  // 初始为启动阶段
//     uint32_t stable_timer = 0;                     // 稳定等待计时器

//     /* 主循环（100ms周期） */
//     for (;;) {
//         // 处理任务间信号
//         task_signal_process();

//         // 1. 加锁读取全局状态（温度、使能等）
//         float T_current = 0.0f;
//         float T_set = 0.0f;
//         temp_status_e temp_status = TEMP_ERROR;
//         traic_status_e traic_en_status = TRAICDISABLE;

//         while (osMutexWait(mutexTempCtrHandle, 100) != osOK);
//         T_current = hmax31865.rtdData.temperature;
//         T_set = temp_ctr.temp_set;
//         temp_status = temp_ctr.tempStatus;
//         traic_en_status = temp_ctr.traicEnStatus;
//         osMutexRelease(mutexTempCtrHandle);

//         // 2. 加热使能状态下的控制逻辑
//         if (traic_en_status == TRAICENABLE) {
//             // 温度异常处理
//             if (temp_status == TEMP_ERROR) {
//                 temp_ctr.traicRunStatus = TRAICSTOP;
//                 temp_ctr.fanRunStatus = FANRUN;
//                 ChangeLed_traic(&last_traic_run);
//                 HAL_GPIO_WritePin(TRAIC_GPIO_Port, TRAIC_Pin, GPIO_PIN_RESET);
//                 HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, GPIO_PIN_SET);
//             } else {
//                 // 3. 计算加热阶段与调功参数
//                 pwm_param_t pwm_param = calc_heating_param(T_current, T_set, 
//                                                           &heating_phase, &stable_timer);

//                 // 极限温度保护
//                 if (pwm_param.is_overlimit) {
//                     temp_ctr.traicRunStatus = TRAICSTOP;
//                     temp_ctr.fanRunStatus = FANRUN;
//                     ChangeLed_traic(&last_traic_run);
//                     HAL_GPIO_WritePin(TRAIC_GPIO_Port, TRAIC_Pin, GPIO_PIN_RESET);
//                     HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, GPIO_PIN_SET);
//                 } else {
//                     // 4. 更新调功周期（周期变化时重置计数器）
//                     if (pwm_period != pwm_param.period) {
//                         pwm_period = pwm_param.period;
//                         pwm_cnt = 0;
//                     }

//                     // 5. 累加周期计数器（100ms/次）
//                     pwm_cnt = (pwm_cnt + 100) % pwm_period;

//                     // 6. 判断当前加热阶段（启动阶段忽略防频繁启停）
//                     bool heating = is_heating_phase(&pwm_param, pwm_cnt);
//                     if (heating_phase == HEATING_INIT) {
//                         // 启动阶段：强制全功率加热
//                         temp_ctr.traicRunStatus = TRAICRUN;
//                         HAL_GPIO_WritePin(TRAIC_GPIO_Port, TRAIC_Pin, GPIO_PIN_SET);
//                         traic_off_delay = 0;  // 重置延迟计数器
//                     } else {
//                         // 非启动阶段：应用防频繁启停逻辑
//                         if (traic_off_delay == 0) {
//                             if (heating) {
//                                 temp_ctr.traicRunStatus = TRAICRUN;
//                                 HAL_GPIO_WritePin(TRAIC_GPIO_Port, TRAIC_Pin, GPIO_PIN_SET);
//                             } else {
//                                 temp_ctr.traicRunStatus = TRAICSTOP;
//                                 HAL_GPIO_WritePin(TRAIC_GPIO_Port, TRAIC_Pin, GPIO_PIN_RESET);
//                                 traic_off_delay = MIN_OFF_TIME;
//                             }
//                         } else {
//                             temp_ctr.traicRunStatus = TRAICSTOP;
//                             HAL_GPIO_WritePin(TRAIC_GPIO_Port, TRAIC_Pin, GPIO_PIN_RESET);
//                             traic_off_delay--;
//                         }
//                     }

//                     // 7. 更新加热指示灯
//                     ChangeLed_traic(&last_traic_run);

//                     // 8. 风扇控制逻辑
//                     if (heating_phase == HEATING_INIT ||  // 启动阶段强制开风扇
//                         T_current >= temp_profile.high_threshold ||  // 高温段
//                         temp_status == HIGHSET) {  // 超温状态
//                         temp_ctr.fanRunStatus = FANRUN;
//                     } else {
//                         temp_ctr.fanRunStatus = FANSTOP;
//                     }
//                 }
//             }
//         } else {
//             // 加热禁用：重置状态（下次使能重新执行启动流程）
//             heating_phase = HEATING_INIT;
//             stable_timer = 0;
//             temp_ctr.traicRunStatus = TRAICSTOP;
//             HAL_GPIO_WritePin(TRAIC_GPIO_Port, TRAIC_Pin, GPIO_PIN_RESET);
//             ChangeLed_traic(&last_traic_run);
//         }

//         // 9. 风扇硬件控制与界面反馈
//         if (temp_ctr.fanEnStatus == FANENABLE) {
//             if (last_fan_run != temp_ctr.fanRunStatus) {
//                 uint32_t retry = 3;
//                 while (retry-- > 0 && osMutexWait(mutexLvglHandle, 50) != osOK);
//                 if (retry >= 0) {
//                     if (temp_ctr.fanRunStatus == FANRUN) {
//                         HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, GPIO_PIN_SET);
//                         lv_obj_set_style_text_color(guider_ui.Hot_Board_HB_label_Fan,
//                                                   lv_color_hex(0x00FF00), LV_PART_MAIN | LV_STATE_DEFAULT);
//                     } else {
//                         HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, GPIO_PIN_RESET);
//                         lv_obj_set_style_text_color(guider_ui.Hot_Board_HB_label_Fan,
//                                                   lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
//                     }
//                     osMutexRelease(mutexLvglHandle);
//                     last_fan_run = temp_ctr.fanRunStatus;
//                 }
//             }
//         } else {
//             HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, GPIO_PIN_RESET);
//         }

//         osDelay(100);  // 100ms任务周期
//     }
// }

// #endif // DRV_TRAIC_H__
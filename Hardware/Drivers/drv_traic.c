#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "gpio.h"
#include "gui_guider.h"
#include "hardware_list.h"
#include "src/widgets/label/lv_label.h"
#include "task.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/_intsup.h>

extern HTRAIcTypeDef htraic;

// 全局实例初始化
HTRAIcTypeDef htraic = {0};

// 参数初始化函数
void TRAIC_ParamInit(void) {
  // 1. 配置核心参数
  htraic.tempProfile.control_period = 2000;       // 控制周期2000ms
  htraic.tempProfile.max_temp_limit = 260.0f;     // 最高260℃
  htraic.tempProfile.min_temp_limit = 1.0f;       // 最低1℃
  htraic.tempProfile.final_stable_delta = 0.4;    // 最终稳定±0.3℃
  htraic.tempProfile.overshoot_threshold = -0.4f; // 超过目标1℃判定为过冲

  htraic.tempProfile.temp_diff_segment[0] = 0.1f;
  htraic.tempProfile.temp_diff_segment[1] = 5;
  htraic.tempProfile.temp_diff_segment[2] = 10;
  htraic.tempProfile.temp_diff_segment[3] = 40;
  htraic.tempProfile.temp_diff_segment[4] = 0;
  htraic.tempProfile.temp_diff_segment[5] = 0;

  htraic.tempProfile.power_pct[0] = 1400;
  htraic.tempProfile.power_pct[1] = 350;
  htraic.tempProfile.power_pct[2] = 250;
  htraic.tempProfile.power_pct[3] = 0;
  htraic.tempProfile.power_pct[4] = 0;
  htraic.tempProfile.power_pct[5] = 0;

  htraic.tempProfile.temp_segment[0] = 0;
  htraic.tempProfile.temp_segment[1] = 55;
  htraic.tempProfile.temp_segment[2] = 85;
  htraic.tempProfile.temp_segment[3] = 125;
  htraic.tempProfile.temp_segment[4] = 185;
  htraic.tempProfile.temp_segment[5] = 225;
  htraic.tempProfile.temp_segment[6] = 235;
  htraic.tempProfile.temp_segment[7] = 250;

  htraic.tempProfile.temp_pct[0] = 70;
  htraic.tempProfile.temp_pct[1] = 100;
  htraic.tempProfile.temp_pct[2] = 120;
  htraic.tempProfile.temp_pct[3] = 140;
  htraic.tempProfile.temp_pct[4] = 160;
  htraic.tempProfile.temp_pct[5] = 180;
  htraic.tempProfile.temp_pct[6] = 200;
  htraic.tempProfile.temp_pct[7] = 240;

  // 3. 动态数据初始化
  htraic.tempCtr.temp_set = 0.0f;     // 默认目标100℃（可外部修改）
  htraic.tempCtr.temp_current = 0.0f; // 当前温度初始值
  htraic.tempCtr.record_cnt = 0;      // 记录计数重置
  htraic.tempCtr.temp_stddev = 0.0f;  // 标准差初始值
  htraic.tempCtr.current_on_ms = 0;   // 导通时间初始0
  for (uint8_t i = 0; i < 4; i++) {
    htraic.tempCtr.temp_history[i] = 0.0f; // 温度记录初始化
  }

  // 4. 硬件状态初始化
  htraic.traicStatus.TRAICENSTATUS = false; // 初始关闭加热
  htraic.traicStatus.TRAICRUNSTATUS = false;
  htraic.fanStatus.FANRUNSTATUS = false;
}

void TRAIC_CalcDynamicPower(void) {
  static uint16_t overshoot_threshold_keep_temp;
  // 判断是否过冲
  float delta = htraic.tempCtr.temp_set -
                htraic.tempCtr.temp_current; // 温差（目标-当前）
  if (delta < htraic.tempProfile.overshoot_threshold) {
    if (htraic.tempCtr.temp_set < 170) 
      htraic.tempCtr.current_on_ms = overshoot_threshold_keep_temp / 2; // 强制停止加热
    else if (htraic.tempCtr.temp_set < 215)
      htraic.tempCtr.current_on_ms+=20;
    else if(htraic.tempCtr.temp_set < 235)
      htraic.tempCtr.current_on_ms+=20;
    else if(htraic.tempCtr.temp_set < 245)
      htraic.tempCtr.current_on_ms+=60;
    htraic.traicStatus.TRAICRUNSTATUS = true;
    return; // 直接返回，不执行后续加热逻辑
  }

  // 判断温差区间
  if (fabsf(delta) > htraic.tempProfile.temp_diff_segment[3]) {
    // 温差大于40，70%功率:周期*0.7
    htraic.tempCtr.current_on_ms = htraic.tempProfile.power_pct[0];
  } else if (fabsf(delta) > htraic.tempProfile.temp_diff_segment[2]) {
    // 温差大于10，20%功率:周期*0.2
    htraic.tempCtr.current_on_ms = htraic.tempProfile.power_pct[1];
  } else if (fabsf(delta) > htraic.tempProfile.temp_diff_segment[1]) {
    // 温差大于5，10%功率:周期*0.1
    htraic.tempCtr.current_on_ms = htraic.tempProfile.power_pct[2];
    if(htraic.tempCtr.temp_set > 235)
      htraic.tempCtr.current_on_ms+=20;
  } else if (fabsf(delta) > htraic.tempProfile.temp_diff_segment[0]) {
    // 判断温度区间
    if (htraic.tempCtr.temp_set < htraic.tempProfile.temp_segment[0]) {
      // 设定值0~55℃
      htraic.tempCtr.current_on_ms =
          htraic.tempProfile.temp_pct[0]; // 5%基准,2000*0.05=100ms
    } else if (htraic.tempCtr.temp_set < htraic.tempProfile.temp_segment[1]) {
      // 设定值55~80℃
      htraic.tempCtr.current_on_ms =
          htraic.tempProfile.temp_pct[1]; // 8%基准,2000*0.07=140ms
    } else if (htraic.tempCtr.temp_set < htraic.tempProfile.temp_segment[2]) {
      // 设定值80~120℃
      htraic.tempCtr.current_on_ms = htraic.tempProfile.temp_pct[2]; // 10%基准
    } else if (htraic.tempCtr.temp_set < htraic.tempProfile.temp_segment[3]) {
      // 设定值120~170℃
      htraic.tempCtr.current_on_ms = htraic.tempProfile.temp_pct[3]; // 10%基准
    } else if (htraic.tempCtr.temp_set < htraic.tempProfile.temp_segment[4]) {
      // 设定值120~170℃
      htraic.tempCtr.current_on_ms = htraic.tempProfile.temp_pct[4]; // 10%基准
    }else if (htraic.tempCtr.temp_set < htraic.tempProfile.temp_segment[5]) {
      // 设定值120~170℃
      htraic.tempCtr.current_on_ms = htraic.tempProfile.temp_pct[5]; // 10%基准
    }else if (htraic.tempCtr.temp_set < htraic.tempProfile.temp_segment[6]) {
      // 设定值120~170℃
      htraic.tempCtr.current_on_ms = htraic.tempProfile.temp_pct[6]; // 10%基准
    }else 
    {
      // 设定值200℃+
      htraic.tempCtr.current_on_ms = htraic.tempProfile.temp_pct[7]; // 15%基准
    }
    overshoot_threshold_keep_temp = htraic.tempCtr.current_on_ms;
  }
}

extern osMutexId_t muteLVGLHandle;

// 执行加热函数（控制硬件输出）
void TRAIC_ExecuteHeating(void) {
  uint32_t off_ms =
      htraic.tempProfile.control_period - htraic.tempCtr.current_on_ms;

  while (osMutexAcquire(muteLVGLHandle, 50) != osOK)
    ;
  char buffer[5];
  snprintf(buffer, 5, "%4d", htraic.tempCtr.current_on_ms);
  lv_label_set_text(guider_ui.Hot_Board_label_duty_time, buffer);
  osMutexRelease(muteLVGLHandle);

  // 导通加热
  if (htraic.tempCtr.current_on_ms > 0 && htraic.traicStatus.TRAICENSTATUS) {
    HAL_GPIO_WritePin(TRAIC_CTL_GPIO_Port, TRAIC_CTL_Pin, GPIO_PIN_SET);
    osDelay(htraic.tempCtr.current_on_ms);
    HAL_GPIO_WritePin(TRAIC_CTL_GPIO_Port, TRAIC_CTL_Pin, GPIO_PIN_RESET);
  }

  // 剩余周期关闭
  if (off_ms > 0) {
    osDelay(off_ms);
  }
}
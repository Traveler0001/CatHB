/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "Drivers/drv_max31865.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "main.h"
#include "task.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "gui_guider.h"
#include "hardware_list.h"
#include "lvgl.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
bool tempChangeFlag = 0;

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for tasklvgl */
osThreadId_t tasklvglHandle;
const osThreadAttr_t tasklvgl_attributes = {
    .name = "tasklvgl",
    .stack_size = 3072 * 4,
    .priority = (osPriority_t)osPriorityLow,
};
/* Definitions for tasktempread */
osThreadId_t tasktempreadHandle;
const osThreadAttr_t tasktempread_attributes = {
    .name = "tasktempread",
    .stack_size = 300 * 4,
    .priority = (osPriority_t)osPriorityLow,
};
/* Definitions for tasktempctr */
osThreadId_t tasktempctrHandle;
const osThreadAttr_t tasktempctr_attributes = {
    .name = "tasktempctr",
    .stack_size = 350 * 4,
    .priority = (osPriority_t)osPriorityLow,
};
/* Definitions for muteLVGL */
osMutexId_t muteLVGLHandle;
const osMutexAttr_t muteLVGL_attributes = {.name = "muteLVGL"};
/* Definitions for mutexMAX31865 */
osMutexId_t mutexMAX31865Handle;
const osMutexAttr_t mutexMAX31865_attributes = {.name = "mutexMAX31865"};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void taskLvgl(void *argument);
void taskTempRead(void *argument);
void taskTempCtr(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationTickHook(void);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 3 */
void vApplicationTickHook(void) {
  /* This function will be called by each tick interrupt if
  configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
  added here, but the tick hook is called from an interrupt context, so
  code must not attempt to block, and only the interrupt safe FreeRTOS API
  functions can be used (those that end in FromISR()). */
  lv_tick_inc(1);
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName) {
  /* Run time stack overflow checking is performed if
  configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
  called if a stack overflow is detected. */
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
}
/* USER CODE END 4 */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of muteLVGL */
  muteLVGLHandle = osMutexNew(&muteLVGL_attributes);

  /* creation of mutexMAX31865 */
  mutexMAX31865Handle = osMutexNew(&mutexMAX31865_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle =
      osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of tasklvgl */
  tasklvglHandle = osThreadNew(taskLvgl, NULL, &tasklvgl_attributes);

  /* creation of tasktempread */
  tasktempreadHandle =
      osThreadNew(taskTempRead, NULL, &tasktempread_attributes);

  /* creation of tasktempctr */
  tasktempctrHandle = osThreadNew(taskTempCtr, NULL, &tasktempctr_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument) {
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin,
                    !HAL_GPIO_ReadPin(LED1_GPIO_Port, LED1_Pin));
  /* Infinite loop */
  for (;;) {
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    // HAL_GPIO_TogglePin(TRAIC_CTL_GPIO_Port, TRAIC_CTL_Pin);
    osDelay(500);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_taskLvgl */
/**
 * @brief Function implementing the tasklvgl thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_taskLvgl */
void taskLvgl(void *argument) {
  /* USER CODE BEGIN taskLvgl */
  /* Infinite loop */
  for (;;) {
    // 获取互斥锁
    while (osMutexAcquire(muteLVGLHandle, 50) != osOK)
      ;
    lv_task_handler(); // 处理LVGL任务
    // 释放互斥锁
    osMutexRelease(muteLVGLHandle);
    osDelay(2);
  }
  /* USER CODE END taskLvgl */
}

/* USER CODE BEGIN Header_taskTempRead */
/**
 * @brief Function implementing the tasktempread thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_taskTempRead */
void taskTempRead(void *argument) {
  /* USER CODE BEGIN taskTempRead */
  static char intBuf[7]; // 整数位缓冲区（固定大小，全局复用）
  static char decBuf[4]; // 小数位缓冲区（固定大小，全局复用）
  /* Infinite loop */
  for (;;) {
    if (uiIndex == UIHOTBOARD) {
      osDelay(3);
      app_MAX31865Read();

      // // 统一获取互斥锁（提前获取，减少重复调用，降低锁竞争频率）
      // while (osMutexAcquire(muteLVGLHandle, 50) != osOK)
      //   ;

      // 统一获取互斥锁（提前获取，减少重复调用，降低锁竞争频率）
      while (osMutexAcquire(muteLVGLHandle, 50) != osOK)
        ;

      if (hmax31865.max_status == MAX31865OK) {
        // 状态正常：更新温度显示
        float temp = hmax31865.max_rtdData.temperature;
        char sign = (temp >= 0) ? ' ' : '-';
        float abs_temp = fabsf(temp);
        uint16_t integer = (uint16_t)floorf(abs_temp);

        // 更新符号（仅变化时刷新）
        const char *currentSymbol =
            lv_label_get_text(guider_ui.Hot_Board_label_symbol);
        if (currentSymbol[0] != sign) {
          lv_label_set_text(guider_ui.Hot_Board_label_symbol,
                            (sign == ' ') ? " " : "-");
        }

        // 更新整数位（复用静态缓冲区，减少栈内存分配）
        snprintf(intBuf, sizeof(intBuf), "%3d", integer);
        const char *currentInt =
            lv_label_get_text(guider_ui.Hot_Board_label_Integer);
        if (currentInt[0] == '-' || currentInt[0] == '.' ||
            atoi(currentInt) != integer) {
          lv_label_set_text(guider_ui.Hot_Board_label_Integer, intBuf);
        }

        // 更新小数位（复用静态缓冲区，简化计算）
        uint8_t decimal = (uint8_t)((abs_temp - integer) * 10);
        snprintf(decBuf, sizeof(decBuf), "%1d", decimal);
        const char *currentDec =
            lv_label_get_text(guider_ui.Hot_Board_label_decimal);
        if (strcmp(currentDec, decBuf) != 0) {
          lv_label_set_text(guider_ui.Hot_Board_label_decimal, decBuf);
        }
      } else if (hmax31865.max_status == MAX31865NOTRADY) {
        // 状态未就绪：显示"---"
        lv_label_set_text(guider_ui.Hot_Board_label_Integer, "---");
        // 可同时清空符号和小数位（避免状态切换时显示残留）
        lv_label_set_text(guider_ui.Hot_Board_label_symbol, " ");
        lv_label_set_text(guider_ui.Hot_Board_label_decimal, " ");
      } else {
        // 其他错误状态：显示".--"
        lv_label_set_text(guider_ui.Hot_Board_label_Integer, ".--");
        // 清空符号和小数位，避免残留
        lv_label_set_text(guider_ui.Hot_Board_label_symbol, " ");
        lv_label_set_text(guider_ui.Hot_Board_label_decimal, " ");
      }

      // 统一释放互斥锁
      osMutexRelease(muteLVGLHandle);
    }
    osDelay(150);
  }
  /* USER CODE END taskTempRead */
}

/* USER CODE BEGIN Header_taskTempCtr */
/**
 * @brief Function implementing the tasktempctr thread.
 * @param argument: Not used
 * @retval None
 */

/* USER CODE END Header_taskTempCtr */
void taskTempCtr(void *argument) {
  /* USER CODE BEGIN taskTempCtr */
  HAL_GPIO_WritePin(TRAIC_CTL_GPIO_Port, TRAIC_CTL_Pin, GPIO_PIN_RESET);
  // 初始化
  TRAIC_ParamInit();
  static float last_temp_set = 0.0f; // 静态变量记录上一次设定值
  // 假设温度传感器初始化（示例）
  // HMAX31865_Init();

  for (;;) {
    // 2. 读取当前温度
    htraic.tempCtr.temp_current = hmax31865.max_rtdData.temperature;
    
    // 1. 加热使能检测
    if (!htraic.traicStatus.TRAICENSTATUS) {
        htraic.tempCtr.current_on_ms = 0;
        htraic.tempCtr.record_cnt = 0;
        osDelay(500);
        continue;
    }

    // 3. 计算占空比
    TRAIC_CalcDynamicPower();
    

    //  // 在读取当前温度后添加：
    // if (fabs(htraic.tempCtr.temp_set - last_temp_set) > 0.1f) {
    //     htraic.temp_set_changed = true;
    //     last_temp_set = htraic.tempCtr.temp_set;
    // }

    // （此处仅为框架，需根据实际传感器驱动实现）

    // 2. 检查设定温度有效性
    // if (htraic.tempCtr.temp_set < htraic.tempProfile.min_temp_limit ||
    //     htraic.tempCtr.temp_set > htraic.tempProfile.max_temp_limit) {
    //   htraic.tempCtr.current_on_ms = 0;
    //   htraic.traicStatus.TRAICRUNSTATUS = false;
    //   osDelay(htraic.tempProfile.control_period);
    //   continue;
    // }

    // 3. 计算功率
    // TRAIC_CalcDynamicPower();

    // 4. 执行加热
    TRAIC_ExecuteHeating();

    // 5. 风扇控制（示例：温度＞100℃或加热时启动）
    htraic.fanStatus.FANRUNSTATUS = (htraic.tempCtr.temp_current > 100.0f) ||
                                    (htraic.traicStatus.TRAICRUNSTATUS);
  }
  /* USER CODE END taskTempCtr */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

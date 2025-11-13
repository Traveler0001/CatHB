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
#include "Drivers/drv_traic.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "main.h"
#include "stm32f4xx_hal_gpio.h"
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
    .stack_size = 256 * 4,
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
    osDelay(200);
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
  htraic.tempProfile = &temp_profile;
  uint32_t last_tick = 0;
  uint16_t cnt = 0;

  /* Infinite loop */
  for (;;) {
    // 0.读取使能状态
    if (htraic.traicStatus.TRAICENSTATUS) {
      // 读取设定温度
      float *tempSet = &htraic.tempCtr.temp_set;
      // 判断设定是否在有效范围
      if (*tempSet > htraic.tempProfile->min_temp_limit && *tempSet < htraic.tempProfile->min_temp_limit)
      {
        // 如果设定温度有效，判断当前温度状态
        // 如果设定温度大于当前温度，修改状态为LOWOFSET
        if (*tempSet > hmax31865.max_rtdData.temperature) 
        {
          htraic.tempStatus = LOWOFSET;
        }
        else
        {
          // 否则设定为HIGHSET
          htraic.tempStatus = HIGHSET;
        }

        // 如果当前温度低于设定温度，根据温差计算加热阶段
        if (htraic.tempStatus == LOWOFSET) 
        {
          // 计算温度差
          float tempDelta = fabsf(*tempSet - hmax31865.max_rtdData.temperature);
          // 根据温差判断当前加热状态
          if (tempDelta > htraic.tempProfile->fullSpeedThreshold) {
            // 温差大于100℃
            htraic.heatSpeed = FULLSPEED;
          } else if (tempDelta > htraic.tempProfile->halfSpeedThreshold) {
            // 温差20~100℃
            htraic.heatSpeed = HALFSPEED;
          } else if(tempDelta > htraic.tempProfile->lowSpeedThreshold){
            // 温差小于20℃
            htraic.heatSpeed = LOWSPEED;
          }else if (tempDelta > htraic.tempProfile->keepSpeedThreshold) {
            htraic.heatSpeed = KEEPSPEED;
          }
        }
        else {
          htraic.heatSpeed = ZEROSPEED;
        }

        if (htraic.heatSpeed == FULLSPEED) {
          HAL_GPIO_WritePin(TRAIC_CTL_GPIO_Port, TRAIC_CTL_Pin, GPIO_PIN_SET);
          osDelay(htraic.tempProfile->fullSpeedThreshold);
          HAL_GPIO_WritePin(TRAIC_CTL_GPIO_Port, TRAIC_CTL_Pin, GPIO_PIN_RESET);
        }else if (htraic.heatSpeed = HALFSPEED) {
          float duty_half = htraic.tempProfile->duty_half / 100.0f * htraic.tempProfile->halfSpeedThreshold;
          HAL_GPIO_WritePin(TRAIC_CTL_GPIO_Port, TRAIC_CTL_Pin, GPIO_PIN_SET);
          osDelay((uint32_t)duty_half);
          HAL_GPIO_WritePin(TRAIC_CTL_GPIO_Port, TRAIC_CTL_Pin, GPIO_PIN_RESET);
          osDelay(htraic.tempProfile->halfSpeedThreshold - (uint32_t)duty_half);
        }else if (htraic.heatSpeed = LOWSPEED) {
          float duty_low = htraic.tempProfile->period_low / 100.0f * htraic.tempProfile->lowSpeedThreshold;
          HAL_GPIO_WritePin(TRAIC_CTL_GPIO_Port, TRAIC_CTL_Pin, GPIO_PIN_SET);
          osDelay((uint32_t)duty_low);
          HAL_GPIO_WritePin(TRAIC_CTL_GPIO_Port, TRAIC_CTL_Pin, GPIO_PIN_RESET);
          osDelay(htraic.tempProfile->lowSpeedThreshold - (uint32_t)duty_low);
        }else if (htraic.heatSpeed = KEEPSPEED) {
          float duty_keep = htraic.tempProfile->period_keep / 100.0f * htraic.tempProfile->keepSpeedThreshold;
          HAL_GPIO_WritePin(TRAIC_CTL_GPIO_Port, TRAIC_CTL_Pin, GPIO_PIN_SET);
          osDelay((uint32_t)duty_keep);
          HAL_GPIO_WritePin(TRAIC_CTL_GPIO_Port, TRAIC_CTL_Pin, GPIO_PIN_RESET);
          osDelay(htraic.tempProfile->keepSpeedThreshold - (uint32_t)duty_keep);
        }
      }
      else {
        htraic.tempStatus = TEMPSETERR;
      }
      

    } else {
    }
    // 2. 加热使能状态下的控制逻辑
    // 3. 计算加热阶段与调功参数
    // 4. 更新调功周期（周期变化时重置计数器）
    // 5. 累加周期计数器（100ms/次）
    // 6. 判断当前加热阶段（启动阶段忽略防频繁启停）
    // 7. 更新加热指示灯
    // 8. 风扇控制逻辑
    // 9. 风扇硬件控制与界面反馈
    // break;

    osDelay(200);
  }
  /* USER CODE END taskTempCtr */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

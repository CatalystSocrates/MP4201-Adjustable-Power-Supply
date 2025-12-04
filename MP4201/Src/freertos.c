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
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "w25qxx.h"
#include "MP4201.h"
#include "mcp4725.h"
#include "screen.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern float vout_target;
extern int text_tag;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MP4201_R_task */
osThreadId_t MP4201_R_taskHandle;
const osThreadAttr_t MP4201_R_task_attributes = {
  .name = "MP4201_R_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for MP4201_W_task */
osThreadId_t MP4201_W_taskHandle;
const osThreadAttr_t MP4201_W_task_attributes = {
  .name = "MP4201_W_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Screen_task */
osThreadId_t Screen_taskHandle;
const osThreadAttr_t Screen_task_attributes = {
  .name = "Screen_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void MP4201_R(void *argument);
void MP4201_W(void *argument);
void Screen(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

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
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of MP4201_R_task */
  MP4201_R_taskHandle = osThreadNew(MP4201_R, NULL, &MP4201_R_task_attributes);

  /* creation of MP4201_W_task */
  MP4201_W_taskHandle = osThreadNew(MP4201_W, NULL, &MP4201_W_task_attributes);

  /* creation of Screen_task */
  Screen_taskHandle = osThreadNew(Screen, NULL, &Screen_task_attributes);

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
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_MP4201_R */
/**
* @brief Function implementing the MP4201_R_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MP4201_R */
void MP4201_R(void *argument)
{
  /* USER CODE BEGIN MP4201_R */
  /* Infinite loop */
  for(;;)
  {
//		mp4201_FB_Mode_set(&MP4201, EXTERNAL_FB);
		get_mp4201_all_info(&MP4201);
		//count++;//每获取一次值就添加到对应total值，次数+1，最后输出值=total/count
    osDelay(1);
  }
  /* USER CODE END MP4201_R */
}

/* USER CODE BEGIN Header_MP4201_W */
/**
* @brief Function implementing the MP4201_W_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MP4201_W */
void MP4201_W(void *argument)
{
  /* USER CODE BEGIN MP4201_W */
  /* Infinite loop */
  for(;;)
  {
		mp4201_vout_set(&MP4201, vout_target);
    osDelay(10);
  }
  /* USER CODE END MP4201_W */
}

/* USER CODE BEGIN Header_Screen */
/**
* @brief Function implementing the Screen_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Screen */
void Screen(void *argument)
{
  /* USER CODE BEGIN Screen */
  /* Infinite loop */
  for(;;)
  {
		screen_update();
    osDelay(10);
  }
  /* USER CODE END Screen */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


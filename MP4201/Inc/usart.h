/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stdio.h"

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart2;

extern UART_HandleTypeDef huart3;

extern UART_HandleTypeDef huart4;

/* USER CODE BEGIN Private defines */
// -------------------------- 协议参数配置--------------------------
#define UART_FRAME_HEAD    0x66        // 帧头
#define UART_FRAME_TAIL1   0xFF        // 帧尾第1字节
#define UART_FRAME_TAIL2   0xFF        // 帧尾第2字节
#define UART_FRAME_TAIL3   0xFF        // 帧尾第3字节
#define BUFFER_MAX_LEN     64          // 环形缓冲区最大长度（需大于最大帧长）
#define MAX_FRAME_LEN      32          // 单帧最大长度（避免无效数据）
// 全局变量声明（供主函数/其他模块调用）
extern uint8_t uart1_recv_frame[MAX_FRAME_LEN];     // 串口1解析结果
static uint8_t recv_data;
// -------------------------- 函数声明 --------------------------
void UART_RingBuffer_Write(uint8_t *ring_buf, uint8_t data);  // 写环形缓冲区
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);
void MX_USART4_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */


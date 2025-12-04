#ifndef SCREEN_H
#define SCREEN_H

#include "main.h"
#include "MP4201Driver.h"
#include "MP4201Command.h"
#include "MP4201Middleware.h"
#include "MP4201.h"
#include "usart.h"

extern hMP4201_t MP4201;

void dataupdate(void);
void screen_update(void);
typedef struct{
    float filterValue;//滤波后的值
    float kalmanGain;//Kalamn增益
    float A;//状态矩阵
    float H;//观测矩阵
    float Q;//状态矩阵的方差
    float R;//观测矩阵的方差
    float P;//预测误差
    float B;
    float u;
}KalmanInfo;

#endif

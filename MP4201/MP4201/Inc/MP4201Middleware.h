/********************************************************************************
* @author: LTS
* @date: 25-7-19 下午1:52
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef MP4201MIDDLEWARE_H
#define MP4201MIDDLEWARE_H

#include "MP4201Command.h"
#include "stdint.h"
#include "stdbool.h"
#include "i2c.h"

#define HARDWARE_I2C

/*设置MP4201地址*/
// #define MP4201_ADDR_GND   0x27 // DEFAULT OPERATION OFF   ADDR = GND
 #define MP4201_ADDR_VCC   0x3F // DEFAULT OPERATION OFF   ADDR = VCC   //EVB配置
// #define MP4201_ADDR_03VCC 0x2F // DEFAULT OPERATION ON    ADDR = 0.3VCC
// #define MP4201_ADDR_06VCC 0x37 // DEFAULT OPERATION ON    ADDR = 0.6VCC

#ifdef MP4201_ADDR_GND
    #define MP4201_ADDR 0x27
#endif
#ifdef MP4201_ADDR_VCC
    #define MP4201_ADDR 0x3F
#endif
#ifdef MP4201_ADDR_03VCC
    #define MP4201_ADDR 0x2F
#endif
#ifdef MP4201_ADDR_06VCC
    #define MP4201_ADDR 0x37
#endif

void MP4201_SendByte(const uint8_t addr, uint8_t command);
void MP4201_WriteByte(const uint8_t addr, uint8_t command, uint8_t data);
void MP4201_WriteWord(const uint8_t addr, uint8_t command, uint16_t data);
void MP4201_ReadByte(const uint8_t addr, uint8_t command, uint8_t *data);
void MP4201_ReadWord(const uint8_t addr, uint8_t command, uint16_t *data);
void MP4201_Delay(uint32_t ms);

#endif //MP4201MIDDLEWARE_H

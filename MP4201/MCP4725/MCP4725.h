#ifndef __MCP4725_H
#define __MCP4725_H

#include "stm32f0xx_hal.h"
#include "main.h"
#include "i2c.h"
#include "MP4201Driver.h"

// MCP4725默认I2C地址 (A0引脚接地)

#define MCP4725_ADDR0 0x60 << 1  // A0=0 MODE
#define MCP4725_ADDR1 0x61 << 1  // A0=1 FREQ


// 操作模式
#define MCP4725_MODE_WRITE 0x40  // 写入DAC寄存器(不保存到EEPROM)
#define MCP4725_MODE_EEPROM 0x60 // 写入DAC寄存器并保存到EEPROM

/**
 * @brief 初始化MCP4725设备
 * @param hi2c I2C句柄指针
 * @param addr 设备I2C地址
 * @return HAL状态
 */
HAL_StatusTypeDef MCP4725_Init(I2C_HandleTypeDef *hi2c, uint8_t addr);

/**
 * @brief 设置MCP4725输出电压
 * @param hi2c I2C句柄指针
 * @param addr 设备I2C地址
 * @param voltage 12位电压值(0-4095)
 * @param save_to_eeprom 是否保存到EEPROM(0=不保存,1=保存)
 * @return HAL状态
 */
HAL_StatusTypeDef MCP4725_SetVoltage(I2C_HandleTypeDef *hi2c, uint8_t addr, 
                                    uint16_t voltage, uint8_t save_to_eeprom);

void MP4201_SET_MODE(MP4201_MODE MODE);
void MP4201_SET_FREQ(MP4201_FREQ FREQ);
/**
 * @brief MCP4725????
 * @param ?
 */
void MCP4725_All_Init(void);
#endif /* __MCP4725_H */
    


#include "mcp4725.h"

/**
 * @brief 初始化MCP4725设备
 * @param hi2c I2C句柄指针
 * @param addr 设备I2C地址
 * @return HAL状态
 */
HAL_StatusTypeDef MCP4725_Init(I2C_HandleTypeDef *hi2c, uint8_t addr) {
    // 检测设备是否在线
    return HAL_I2C_IsDeviceReady(hi2c, addr, 3, 100);
}

void MCP4725_All_Init(void)
{
	MCP4725_Init(&hi2c1, MCP4725_ADDR0);
	MCP4725_Init(&hi2c1, MCP4725_ADDR1);
}
/**
 * @brief 设置MCP4725输出电压
 * @param hi2c I2C句柄指针
 * @param addr 设备I2C地址
 * @param voltage 12位电压值(0-4095)
 * @param save_to_eeprom 是否保存到EEPROM(0=不保存,1=保存)
 * @return HAL状态
 */
HAL_StatusTypeDef MCP4725_SetVoltage(I2C_HandleTypeDef *hi2c, uint8_t addr, 
                                    uint16_t voltage, uint8_t save_to_eeprom) {
    uint8_t data[3];
    uint8_t mode;
    
    // 限制电压值在0-4095范围内
    if (voltage > 4095) {
        voltage = 4095;
    }
    
    // 选择操作模式
    mode = save_to_eeprom ? MCP4725_MODE_EEPROM : MCP4725_MODE_WRITE;
    
    // 构建数据帧
    data[0] = mode;                      // 控制字节
    data[1] = (voltage >> 4) & 0xFF;     // 高8位数据
    data[2] = (voltage << 4) & 0xF0;     // 低4位数据
    
    // 通过I2C发送数据
    return HAL_I2C_Master_Transmit(hi2c, addr, data, 3, 100);
}
						
void MP4201_SET_MODE(MP4201_MODE MODE)
{
	MCP4725_SetVoltage(&hi2c1,MCP4725_ADDR0,MODE,1);
}
void MP4201_SET_FREQ(MP4201_FREQ FREQ)
{
	MCP4725_SetVoltage(&hi2c1,MCP4725_ADDR1,FREQ,1);
}

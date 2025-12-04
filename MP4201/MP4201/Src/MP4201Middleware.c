/********************************************************************************
* @author: LTS
* @date: 25-7-19 下午1:52
* @version: 1.0
* @description: 
********************************************************************************/
#include "MP4201Middleware.h"

void MP4201_Delay(uint32_t ms)
{
    HAL_Delay(ms);
}

#ifdef HARDWARE_I2C
    /**
     * @brief 向 MP4201 发送一个字节
     * @param addr MP4201 I2C 地址
     * @param command
     */
    void MP4201_SendByte(const uint8_t addr, uint8_t command)
    {
        HAL_I2C_Master_Transmit(&hi2c1, (addr << 1), &command, 1, HAL_MAX_DELAY);
    }

    /**
     * @brief 向 MP4201 寄存器写入一个字节
     * @param addr MP4201 I2C地址
     * @param command MP4201 从设备指令
     * @param data 写入 MP4201 寄存器的8位数据
     */
    void MP4201_WriteByte(const uint8_t addr, uint8_t command, uint8_t data)
    {
        HAL_I2C_Mem_Write(&hi2c1, addr << 1, command, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    }

    /**
     * 向 MP4201 寄存器写入一个字
     * @param addr MP4201 I2C地址
     * @param command MP4201 从设备指令
     * @param data 写入 MP4201 寄存器的16位数据
     */
    void MP4201_WriteWord(const uint8_t addr, uint8_t command, uint16_t data)
    {
        // uint8_t data_8bit[2] = {(uint8_t)data, data >> 8};
        uint8_t data_8bit[2] = {0x00};
        data_8bit[0] = (uint8_t)data;
        data_8bit[1] = data >> 8;
        HAL_I2C_Mem_Write(&hi2c1, addr << 1, command, I2C_MEMADD_SIZE_8BIT, data_8bit, 2, HAL_MAX_DELAY);
    }

    /**
     * 向 MP4201 寄存器读取一个字节
     * @param addr MP4201 I2C地址
     * @param command MP4201 从设备指令
     * @param data 读取 MP4201 寄存器的8位数据
     */
    void MP4201_ReadByte(const uint8_t addr, uint8_t command, uint8_t *data)
    {
        HAL_I2C_Mem_Read(&hi2c1, addr << 1, command, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);
    }

    /**
     * 向 MP4201 寄存器读取一个字
     * @param addr MP4201 I2C地址
     * @param command MP4201 从设备指令
     * @param data 读取 MP4201 寄存器的16位数据
     */
    void MP4201_ReadWord(const uint8_t addr, uint8_t command, uint16_t *data)
    {
        uint8_t data_8bit[2] = {0x00};
        HAL_I2C_Mem_Read(&hi2c1, addr << 1, command, I2C_MEMADD_SIZE_8BIT, data_8bit, 2, HAL_MAX_DELAY);
        *data = (uint16_t)((data_8bit[1] << 8) | data_8bit[0]);
    }
#endif

#ifdef SOFTWARE_I2C
    /**
     * @brief 向 MP4201 发送一个字节
     * @param addr MP4201 I2C 地址
     * @param command
     */
    void MP4201_SendByte(const uint8_t addr, uint8_t command)
    {
        bool Is_no_ACK = 0;

        IIC_Start();
        IIC_Send(addr << 1 | 0x01); // write addr
        Is_no_ACK = IIC_Wait_Ack();
        if (Is_no_ACK)
            return;

        IIC_Send(command);
        Is_no_ACK = IIC_Wait_Ack();
        if (Is_no_ACK)
            return;
        IIC_Stop();
    }

    /**
     * @brief 向 MP4201 寄存器写入一个字节
     * @param addr MP4201 I2C地址
     * @param command MP4201 从设备指令
     * @param data 写入 MP4201 寄存器的8位数据
     */
    void MP4201_WriteByte(const uint8_t addr, uint8_t command, uint8_t data)
    {
        bool Is_no_ACK = 0;

        IIC_Start();
        IIC_Send(addr << 1 | 0x01); // write addr
        Is_no_ACK = IIC_Wait_Ack();
        if (Is_no_ACK)
            return;

        IIC_Send(command);
        Is_no_ACK = IIC_Wait_Ack();
        if (Is_no_ACK)
            return;

        IIC_Send(data);
        Is_no_ACK = IIC_Wait_Ack();
        if (Is_no_ACK)
            return;
        IIC_Stop();
    }

    /**
     * 向 MP4201 寄存器写入一个字
     * @param addr MP4201 I2C地址
     * @param command MP4201 从设备指令
     * @param data 写入 MP4201 寄存器的16位数据
     */
    void MP4201_WriteWord(const uint8_t addr, uint8_t command, uint16_t data)
    {
        bool Is_no_ACK = 0;
        uint8_t data_8bit[2] = {0x00};
        data_8bit[0] = (uint8_t)data;
        data_8bit[1] = data >> 8;

        IIC_Start();
        IIC_Send(addr << 1 | 0x01); // write addr
        Is_no_ACK = IIC_Wait_Ack();
        if (Is_no_ACK)
            return;

        IIC_Send(command);
        Is_no_ACK = IIC_Wait_Ack();
        if (Is_no_ACK)
            return;

        IIC_Send(data_8bit[0]);
        Is_no_ACK = IIC_Wait_Ack();
        if (Is_no_ACK)
            return;

        IIC_Send(data_8bit[1]);
        Is_no_ACK = IIC_Wait_Ack();
        if (Is_no_ACK)
            return;
        IIC_Stop();
    }

    /**
     * 向 MP4201 寄存器读取一个字节
     * @param addr MP4201 I2C地址
     * @param command MP4201 从设备指令
     * @param data 读取 MP4201 寄存器的8位数据
     */
    void MP4201_ReadByte(const uint8_t addr, uint8_t command, uint8_t *data)
    {
        bool Is_no_ACK = 0;

        IIC_Start();
        IIC_Send(addr << 1 | 0x01); // write addr
        Is_no_ACK = IIC_Wait_Ack();
        if (Is_no_ACK)
            return;

        IIC_Send(command);
        Is_no_ACK = IIC_Wait_Ack();
        if (Is_no_ACK)
            return;

        IIC_Send(addr << 1 | 0x00); // read addr
        Is_no_ACK = IIC_Wait_Ack();
        if (Is_no_ACK)
            return;

        *data = IIC_Read();
        IIC_Stop();
    }

    /**
     * 向 MP4201 寄存器读取一个字
     * @param addr MP4201 I2C地址
     * @param command MP4201 从设备指令
     * @param data 读取 MP4201 寄存器的16位数据
     */
    void MP4201_ReadWord(const uint8_t addr, uint8_t command, uint16_t *data)
    {
        uint8_t data_8bit[2] = {0x00};
        bool Is_no_ACK = 0;

        IIC_Start();
        IIC_Send(addr << 1 | 0x01); // write addr
        Is_no_ACK = IIC_Wait_Ack();
        if (Is_no_ACK)
            return;

        IIC_Send(command);
        Is_no_ACK = IIC_Wait_Ack();
        if (Is_no_ACK)
            return;

        IIC_Send(addr << 1 | 0x00); // read addr
        Is_no_ACK = IIC_Wait_Ack();
        if (Is_no_ACK)
            return;
        data_8bit[1] = IIC_Read();
        IIC_Ack(0);

        data_8bit[0] = IIC_Read();
        IIC_Ack(0);
        IIC_Stop();

        *data = (uint16_t)(data_8bit[1] << 8 | data_8bit[0]);
    }
#endif

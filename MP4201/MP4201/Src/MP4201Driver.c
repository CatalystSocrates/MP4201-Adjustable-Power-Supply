/********************************************************************************
* @author: LTS
* @date: 25-7-19 下午1:52
* @version: 1.0
* @description:
********************************************************************************/
#include "MP4201Driver.h"
#include "MP4201.h"
uint8_t MFR_CTRL1_REG_value;
uint8_t MFR_CTRL1_reg_value;

static  hMP4201_I2C_Address_e transfer_MP4201_t_device_address(uint8_t addr);
/**
 * @brief 设置MP4201输出,默认输出,可用status控制是否输出
 * @param mp4201,status
 */
void mp4201_operation_set(hMP4201_t *mp4201, bool status)
{
    mp4201->operation = status;
    MP4201_WriteByte(mp4201->Device_Address,OPERATION_COMMAND_CODE,status << 7);
}
/**
 * @brief 读取MP4201输出状态
 * @param mp4201
 */
void mp4201_read_operation_status(hMP4201_t *mp4201)
{
    uint8_t data = 0;
    MP4201_ReadByte(mp4201->Device_Address,OPERATION_COMMAND_CODE, &data);
    mp4201->operation = data >> 7 & 0x01;
}
/**
 * @brief 配置MP4201输出电压
 * @param mp4201,vout
 * @note 使用前最好配置使用内部电阻
 */
void mp4201_vout_set(hMP4201_t *mp4201, float vout)
{
    if (vout < 4.0f)
		{
			mp4201_FB_Mode_set(&MP4201, EXTERNAL_FB);
		}
		else if (vout >= 4.0f)
		{
			mp4201_FB_Mode_set(&MP4201, INTERNAL_FB);
		}
	  if (mp4201->MFR_CTRL1.FB_MODE == INTERNAL_FB)
    {
        if (vout < 0.1f)
            vout = 0.1f;
        else if (vout > 85.12f)
            vout = 85.12f;
        mp4201->Vout_set = vout;
        uint16_t vout_reg_val = (uint16_t)((vout * 1000.0f / MP4201_INTERNAL_FEEDBACK_RATIO) / 0.625f) & 0x0FFF;
        MP4201_WriteWord(mp4201->Device_Address,VOUT_COMMAND_COMMAND_CODE,vout_reg_val);
    }
    else if (mp4201->MFR_CTRL1.FB_MODE == EXTERNAL_FB)
    {
        if (vout < 0.1f)
            vout = 0.1f;
        else if (vout > 100.0f)
            vout = 100.0f;
				uint16_t vout_reg_val = (uint16_t)((vout * 1000.0f / EXTERNAL_FEEDBACK_RATIO) / 0.625f) & 0x0FFF;
        mp4201->Vout_set = vout;
        MP4201_WriteWord(mp4201->Device_Address,VOUT_COMMAND_COMMAND_CODE,vout_reg_val);
    }
}
/**
 * @brief 读取配置的MP4201输出电压
 * @param mp4201
 */
void mp4201_read_vout_set(hMP4201_t *mp4201)
{
    uint16_t vout_reg_val = 0;
    if (mp4201->MFR_CTRL1.FB_MODE == INTERNAL_FB)
    {
        MP4201_ReadWord(mp4201->Device_Address, VOUT_COMMAND_COMMAND_CODE, &vout_reg_val);
				mp4201->vout_reg_val = vout_reg_val;
        mp4201->Vout_set = (float)vout_reg_val * 0.625f * MP4201_INTERNAL_FEEDBACK_RATIO / 1000.0f;
    }
    else if (mp4201->MFR_CTRL1.FB_MODE == EXTERNAL_FB)
    {
        MP4201_ReadWord(mp4201->Device_Address, VOUT_COMMAND_COMMAND_CODE, &vout_reg_val);
				mp4201->vout_reg_val = vout_reg_val;
        mp4201->Vout_set = (float)vout_reg_val * 0.625f * mp4201->external_feedback_ratio / 1000.0f;
    }
}
/**
 * @brief 配置MP4201输出调节阈值电压
 * @param mp4201,vin_reg_thld_value
 */
void mp4201_vin_reg_thld_set(hMP4201_t *mp4201, float vin_reg_thld_value)
{
    uint16_t vin_reg_thld_value_reg_val = 0;
    mp4201->Vin_THLD = vin_reg_thld_value;
    if (mp4201->MFR_CTRL1.FB_MODE == INTERNAL_FB)
    {
        if (vin_reg_thld_value < 0.1f)
            vin_reg_thld_value = 0.1f;
        else if (vin_reg_thld_value > 85.12f)
            vin_reg_thld_value = 85.12f;
        vin_reg_thld_value_reg_val = (uint16_t)((vin_reg_thld_value * 1000.0f / MP4201_INTERNAL_FEEDBACK_RATIO) / 0.625f) & 0x0FFF;
        MP4201_WriteWord(mp4201->Device_Address,VIN_REG_THLD_COMMAND_CODE,vin_reg_thld_value_reg_val);
    }
    else if (mp4201->MFR_CTRL1.FB_MODE == EXTERNAL_FB)
    {
        if (vin_reg_thld_value < 0.1f)
            vin_reg_thld_value = 0.1f;
        else if (vin_reg_thld_value > 100.0f)
            vin_reg_thld_value = 100.0f;
        vin_reg_thld_value_reg_val = (uint16_t)(vin_reg_thld_value * 1000.0f / mp4201->external_feedback_ratio / 0.625f) & 0x0FFF;
        MP4201_WriteWord(mp4201->Device_Address,VIN_REG_THLD_COMMAND_CODE,vin_reg_thld_value_reg_val);
    }
}
/**
 * @brief 读取配置的MP4201输出调节阈值电压
 * @param mp4201
 */
void mp4201_read_vin_reg_thld_set(hMP4201_t *mp4201)
{
    uint16_t vin_reg_thld_value_reg_val = 0;
    if (mp4201->MFR_CTRL1.FB_MODE == INTERNAL_FB)
    {
        MP4201_ReadWord(mp4201->Device_Address, VIN_REG_THLD_COMMAND_CODE, &vin_reg_thld_value_reg_val);
        mp4201->Vin_THLD = (float)vin_reg_thld_value_reg_val * 0.625f * MP4201_INTERNAL_FEEDBACK_RATIO / 1000.0f;
    }
    else if (mp4201->MFR_CTRL1.FB_MODE == EXTERNAL_FB)
    {
        MP4201_ReadWord(mp4201->Device_Address, VIN_REG_THLD_COMMAND_CODE, &vin_reg_thld_value_reg_val);
        mp4201->Vin_THLD = (float)vin_reg_thld_value_reg_val * 0.625f * mp4201->external_feedback_ratio / 1000.0f;
    }

}
/**
 * @brief 配置MP4201输出限流
 * @param mp4201,iout_oc_fault_limit_value
 */
void mp4201_iout_oc_fault_limit_set(hMP4201_t *mp4201, float iout_oc_fault_limit_value)
{
    if (iout_oc_fault_limit_value < 0.0f)
        iout_oc_fault_limit_value = 0.0f;
    else if (iout_oc_fault_limit_value > 25.0f)
        iout_oc_fault_limit_value = 25.0f;
    mp4201->I_output_overcurrent_limit_value = iout_oc_fault_limit_value;
    uint16_t iout_oc_fault_limit_value_reg_val = (uint16_t)(iout_oc_fault_limit_value * 20.0f) & 0x01FF;
    MP4201_WriteWord(mp4201->Device_Address,IOUT_OC_FAULT_LIMIT_COMMAND_CODE,iout_oc_fault_limit_value_reg_val);
}
/**
 * @brief 读取配置的MP4201输出限流值
 * @param mp4201
 */
void mp4201_read_iout_oc_fault_limit_set(hMP4201_t *mp4201)
{
    uint16_t iout_oc_fault_limit_value_reg_val = 0;
    MP4201_ReadWord(mp4201->Device_Address, IOUT_OC_FAULT_LIMIT_COMMAND_CODE, &iout_oc_fault_limit_value_reg_val);
    mp4201->I_output_overcurrent_limit_value = (float)iout_oc_fault_limit_value_reg_val * 0.05f;
}

/**
 * @brief 配置MP4201输入限流
 * @param mp4201,iout_oc_fault_limit_value
 */
void mp4201_iin_oc_fault_limit_set(hMP4201_t *mp4201, float iin_oc_fault_limit_value)
{
    if (iin_oc_fault_limit_value < 0.0f)
        iin_oc_fault_limit_value = 0.0f;
    else if (iin_oc_fault_limit_value > 25.0f)
        iin_oc_fault_limit_value = 25.0f;
    mp4201->I_input_overcurrent_limit_value = iin_oc_fault_limit_value;
    uint16_t iin_oc_fault_limit_value_reg_val = ((uint16_t)(iin_oc_fault_limit_value * 20.0f) & 0x01FF);
    MP4201_WriteWord(mp4201->Device_Address,IIN_OC_FAULT_LIMIT_COMMAND_CODE,iin_oc_fault_limit_value_reg_val);
}
/**
 * @brief 读取配置的MP4201输入限流值
 * @param mp4201
 */
void mp4201_read_iin_oc_fault_limit_set(hMP4201_t *mp4201)
{
    uint16_t iin_oc_fault_limit_value_reg_val = 0;

    MP4201_ReadWord(mp4201->Device_Address, IIN_OC_FAULT_LIMIT_COMMAND_CODE, &iin_oc_fault_limit_value_reg_val);
    mp4201->I_input_overcurrent_limit_value = (float)iin_oc_fault_limit_value_reg_val * 0.05f;
}
/**
 * @brief 读取MP4201的各种状态标志位
 * @param mp4201
 */
void get_mp4201_status_info(hMP4201_t *mp4201)
{
    uint16_t status_word = 0;
    MP4201_ReadWord(mp4201->Device_Address, STATUS_WORD_COMMAND_CODE, &status_word);

    mp4201->status_info.SCP_fault              = (status_word & 0x04000) >> 13;   //短路报警
    mp4201->status_info.Vin_overvoltage_fault  = (status_word & 0x02000) >> 12;   //输入过压
    mp4201->status_info.PG_STATUS              = (status_word & 0x0800) >> 10;    //输出异常
    mp4201->status_info.Is_Charger_completed   = (status_word & 0x0200) >> 8;     //充电完成标志位
    mp4201->status_info.Iin_overcurrent_fault  = (status_word & 0x0100) >> 7;     //输入过流
    mp4201->status_info.Vout_overvoltage_fault = (status_word & 0x0020) >> 4;     //输出过压
    mp4201->status_info.Iout_overcurrent_fault = (status_word & 0x0010) >> 3;     //输出过流
    mp4201->status_info.Temperature_fault      = (status_word & 0x0004) >> 1;     //温度过高标志位
    mp4201->status_info.CRC_error_fault        = (status_word & 0x0001);          //CRC校验异常
}
/**
 * @brief 清除MP4201的各种状态标志位
 * @param mp4201
 */
void mp4201_clear_faults(hMP4201_t *mp4201)
{
    MP4201_SendByte(mp4201->Device_Address,CLEAR_FAULTS_COMMAND_CODE);
    mp4201->status_info.SCP_fault              = false;
    mp4201->status_info.Vin_overvoltage_fault  = false;
    mp4201->status_info.PG_STATUS              = false;
    mp4201->status_info.Is_Charger_completed   = false;
    mp4201->status_info.Iin_overcurrent_fault  = false;
    mp4201->status_info.Vout_overvoltage_fault = false;
    mp4201->status_info.Iout_overcurrent_fault = false;
    mp4201->status_info.Temperature_fault      = false;
    mp4201->status_info.CRC_error_fault        = false;
}
/**
 * @brief 读取MP4201温度参数
 * @param mp4201
 */
void get_mp4201_status_temperature_info(hMP4201_t *mp4201)
{
    uint8_t status_temperature = 0;
    MP4201_ReadByte(mp4201->Device_Address, STATUS_TEMPER_ATURE_COMMAND_CODE, &status_temperature);

    mp4201->temperature_info.OTP_fault         = (status_temperature & 0x80) >> 3;
    mp4201->temperature_info.OTP_warning_fault = (status_temperature & 0x40) >> 2;
    mp4201->temperature_info.NTC_fault         = (status_temperature & 0x20);
}
/**
 * @brief 读取MP4201输入电压
 * @param mp4201
 */
void get_mp4201_vin_info(hMP4201_t *mp4201)
{
    uint16_t vin = 0;
    MP4201_ReadWord(mp4201->Device_Address,READ_VIN_COMMAND_CODE, &vin);
    mp4201->read_info.Vin_read = (float)(vin & 0x3FF) * 0.0832f;
}
/**
 * @brief 读取MP4201输入电压寄存器值
 * @param mp4201
 */
uint16_t get_mp4201_vin_reg_info(hMP4201_t *mp4201)
{
	uint16_t vin = 0;
	MP4201_ReadWord(mp4201->Device_Address,READ_VIN_COMMAND_CODE, &vin);
	return vin;
}
/**
 * @brief 读取MP4201平均输入电压
 * @param mp4201
 */
void get_mp4201_average_vin_info(hMP4201_t *mp4201, uint8_t count)
{
    float vin_average = 0.0f;
    for (int i = 0; i < count; i++)
    {
        uint16_t vin = 0;
        MP4201_ReadWord(mp4201->Device_Address,READ_VIN_COMMAND_CODE, &vin);
        vin_average += (float)(vin & 0x3FF) * 0.0832f;
		MP4201_Delay(2);
    }
    vin_average /= (float)count;
    mp4201->read_info.Vin_read = vin_average;
}
/**
 * @brief 读取MP4201输入电流
 * @param mp4201
 */
void get_mp4201_iin_info(hMP4201_t *mp4201)
{
    uint16_t iin = 0;
    MP4201_ReadWord(mp4201->Device_Address,READ_IIN_COMMAND_CODE, &iin);
    mp4201->read_info.Iin_read = (float)(iin & 0x3FF) * 0.043f;
}
/**
 * @brief 读取MP4201输入电流寄存器值
 * @param mp4201
 */
uint16_t get_mp4201_iin_reg_info(hMP4201_t *mp4201)
{
	uint16_t iin = 0;
	MP4201_ReadWord(mp4201->Device_Address,READ_IIN_COMMAND_CODE, &iin);
	return iin;
}
/**
 * @brief 读取MP4201平均输入电流
 * @param mp4201
 */
void get_mp4201_average_iin_info(hMP4201_t *mp4201, uint8_t count)
{
	
    float iin_average = 0.0f;
    for (int i = 0; i < count; i++)
    {
        uint16_t iin = 0;
        MP4201_ReadWord(mp4201->Device_Address,READ_IIN_COMMAND_CODE, &iin);

        iin_average += (float)(iin & 0x3FF) * 0.043f;
		MP4201_Delay(2);
    }
    iin_average /= (float)count;
    mp4201->read_info.Iin_read = iin_average;
}
/**
 * @brief 读取MP4201输出电压
 * @param mp4201
 */
void get_mp4201_vout_info(hMP4201_t *mp4201)
{
    uint16_t vout = 0;
    MP4201_ReadWord(mp4201->Device_Address,READ_VOUT_COMMAND_CODE, &vout);
    mp4201->read_info.Vout_read = (float)(vout & 0x3FF) * 0.0832f;
}
/**
 * @brief 读取MP4201输出电压寄存器值
 * @param mp4201
 */
uint16_t get_mp4201_vout_reg_info(hMP4201_t *mp4201)
{
	uint16_t vout = 0;
	MP4201_ReadWord(mp4201->Device_Address,READ_VOUT_COMMAND_CODE, &vout);
	return vout;
}
/**
 * @brief 读取MP4201输出电压平均值
 * @param mp4201
 */
void get_mp4201_average_vout_info(hMP4201_t *mp4201, uint8_t count)
{
    float vout_average = 0.0f;
    for (int i = 0; i < count; i++)
    {
        uint16_t vout = 0;
        MP4201_ReadWord(mp4201->Device_Address,READ_VOUT_COMMAND_CODE, &vout);
        vout_average += (float)(vout & 0x3FF) * 0.0832f;
		MP4201_Delay(2);
    }
    vout_average /= (float)count;
    mp4201->read_info.Vout_read = vout_average;
}
/**
 * @brief 读取MP4201输出电流
 * @param mp4201
 */
void get_mp4201_iout_info(hMP4201_t *mp4201)
{
    uint16_t iout = 0;
    MP4201_ReadWord(mp4201->Device_Address,READ_IOUT_COMMAND_CODE, &iout);
    mp4201->read_info.Iout_read = (float)(iout & 0x3FF) * 0.043f;
}
/**
 * @brief 读取MP4201输出电流寄存器的值
 * @param mp4201
 */
uint16_t get_mp4201_iout_reg_info(hMP4201_t *mp4201)
{
	uint16_t iout = 0;
	MP4201_ReadWord(mp4201->Device_Address,READ_IOUT_COMMAND_CODE, &iout);
	return iout;
}
/**
 * @brief 读取MP4201输出电流多轮平均值
 * @param mp4201
 */
void get_mp4201_average_iout_info(hMP4201_t *mp4201, uint8_t count)
{
    float iout_average = 0.0f;
    for (int i = 0; i < count; i++)
    {
        uint16_t iout = 0;
        MP4201_ReadWord(mp4201->Device_Address,READ_IOUT_COMMAND_CODE, &iout);
        iout_average += (float)(iout & 0x3FF) * 0.043f;
		MP4201_Delay(2);
    }
    iout_average /= (float)count;
    mp4201->read_info.Iout_read = iout_average;
}
/**
 * @brief 读取MP4201温度
 * @param mp4201
 */
void get_mp4201_temperature_info(hMP4201_t *mp4201)
{
    uint16_t temperature = 0;
    MP4201_ReadWord(mp4201->Device_Address,READ_TEMPERATURE_COMMAND_CODE, &temperature);
    mp4201->read_info.Temperature_read = -272.48f + 1.07f * (float)temperature;
}
/**
 * @brief 读取MP4201温度多轮平均值
 * @param mp4201
 */
void get_mp4201_average_temperature_info(hMP4201_t *mp4201, uint8_t count)
{
    float temperature_average = 0.0f;
    for (int i = 0; i < count; i++)
    {
        uint16_t temperature = 0;
        MP4201_ReadWord(mp4201->Device_Address,READ_TEMPERATURE_COMMAND_CODE, &temperature);
        temperature_average += -272.48f + 1.07f * (float)temperature;
		MP4201_Delay(2);
    }
    temperature_average /= (float)count;
    mp4201->read_info.Temperature_read = temperature_average;
}

//********************************MFR_CTRL1 REG********************************//

void get_mp4201_MFR_CTRL1_reg_info(hMP4201_t *mp4201)
{
    MP4201_ReadByte(mp4201->Device_Address,MFR_CTRL1_COMMAND_CODE, &MFR_CTRL1_REG_value);
    
    mp4201->MFR_CTRL1.DIR_CONFIG     = (MFR_CTRL1_REG_value & 0x80) >> 7;
    mp4201->MFR_CTRL1.Input_UVLO_SEL = (MFR_CTRL1_REG_value & 0x60) >> 5;
    mp4201->MFR_CTRL1.GPIO1_GATE_EN  = (MFR_CTRL1_REG_value & 0x10) >> 4;
    mp4201->MFR_CTRL1.FB_MODE        = (MFR_CTRL1_REG_value & 0x08) >> 3;
    mp4201->MFR_CTRL1.OCP_MODE       = (MFR_CTRL1_REG_value & 0x04) >> 2;
    mp4201->MFR_CTRL1.DEAD_TIME      = (MFR_CTRL1_REG_value & 0x03);
}


void write_mp4201_MFR_CTRL1_reg(hMP4201_t *mp4201, uint8_t MFR_CTRL1_reg_value)
{
    MP4201_WriteByte(mp4201->Device_Address,MFR_CTRL1_COMMAND_CODE,MFR_CTRL1_reg_value);
    get_mp4201_MFR_CTRL1_reg_info(mp4201);
}

void Reset_mp4201_MFR_CTRL1(hMP4201_t *mp4201)
{
    MP4201_WriteByte(mp4201->Device_Address,MFR_CTRL1_COMMAND_CODE,0x09);
    get_mp4201_MFR_CTRL1_reg_info(mp4201);
}

uint8_t transfer_mp4201_MFR_CTRL1_reg(const hMP4201_MFR_CTRL1_t *MFR_CTRL1)
{
//    uint8_t MFR_CTRL1_reg_value = 0x09;
		MFR_CTRL1_reg_value = MFR_CTRL1_REG_value;
    MFR_CTRL1_reg_value = MFR_CTRL1_reg_value | MFR_CTRL1->DIR_CONFIG << 7;
    MFR_CTRL1_reg_value = MFR_CTRL1_reg_value | MFR_CTRL1->Input_UVLO_SEL << 5;
    MFR_CTRL1_reg_value = MFR_CTRL1_reg_value | MFR_CTRL1->GPIO1_GATE_EN << 4;
    MFR_CTRL1_reg_value = MFR_CTRL1_reg_value | MFR_CTRL1->FB_MODE << 3;
    MFR_CTRL1_reg_value = MFR_CTRL1_reg_value | MFR_CTRL1->OCP_MODE << 2;
    MFR_CTRL1_reg_value = MFR_CTRL1_reg_value | MFR_CTRL1->DEAD_TIME;
    return MFR_CTRL1_reg_value;
}

void mp4201_DIR_set(hMP4201_t *mp4201, bool dir)
{
//    uint8_t MFR_CTRL1_reg_value = 0x09;
		
    mp4201->MFR_CTRL1.DIR_CONFIG = dir;
    MFR_CTRL1_reg_value = transfer_mp4201_MFR_CTRL1_reg(&mp4201->MFR_CTRL1);
    write_mp4201_MFR_CTRL1_reg(mp4201, MFR_CTRL1_reg_value);
}

void mp4201_Input_UVLO_set(hMP4201_t *mp4201, hMP4201_Input_UVLO_e input_uvlo_sel)
{
//    uint8_t MFR_CTRL1_reg_value = 0x09;
    mp4201->MFR_CTRL1.Input_UVLO_SEL = input_uvlo_sel;
    MFR_CTRL1_reg_value = transfer_mp4201_MFR_CTRL1_reg(&mp4201->MFR_CTRL1);
    write_mp4201_MFR_CTRL1_reg(mp4201, MFR_CTRL1_reg_value);
}

void mp4201_GPIO1_GATE_enable_set(hMP4201_t *mp4201, bool status)
{
//    uint8_t MFR_CTRL1_reg_value = 0x09;
    mp4201->MFR_CTRL1.GPIO1_GATE_EN = status;
    MFR_CTRL1_reg_value = transfer_mp4201_MFR_CTRL1_reg(&mp4201->MFR_CTRL1);
    write_mp4201_MFR_CTRL1_reg(mp4201, MFR_CTRL1_reg_value);
}

void mp4201_FB_Mode_set(hMP4201_t *mp4201, hMP4201_FB_MODE_e fb_mode)
{
//    uint8_t MFR_CTRL1_reg_value = 0x09;
		MFR_CTRL1_reg_value=MFR_CTRL1_REG_value;
		MFR_CTRL1_reg_value=MFR_CTRL1_reg_value&0xF7;
		MFR_CTRL1_reg_value=MFR_CTRL1_reg_value|fb_mode<<3;
    mp4201->MFR_CTRL1.FB_MODE = fb_mode;
//    MFR_CTRL1_reg_value = transfer_mp4201_MFR_CTRL1_reg(&mp4201->MFR_CTRL1);
    write_mp4201_MFR_CTRL1_reg(mp4201, MFR_CTRL1_reg_value);
}

void mp4201_OCP_Mode_set(hMP4201_t *mp4201, hMP4201_OCP_MODE_e ocp_mode)
{
//    uint8_t MFR_CTRL1_reg_value = 0x09;
    mp4201->MFR_CTRL1.OCP_MODE = ocp_mode;
    MFR_CTRL1_reg_value = transfer_mp4201_MFR_CTRL1_reg(&mp4201->MFR_CTRL1);
    write_mp4201_MFR_CTRL1_reg(mp4201, MFR_CTRL1_reg_value);
}

void mp4201_DEAD_Time_set(hMP4201_t *mp4201, hMP4201_DEAD_TIME_e dead_time)
{
//    uint8_t MFR_CTRL1_reg_value = 0x09;
    mp4201->MFR_CTRL1.DEAD_TIME = dead_time;
    MFR_CTRL1_reg_value = transfer_mp4201_MFR_CTRL1_reg(&mp4201->MFR_CTRL1);
    write_mp4201_MFR_CTRL1_reg(mp4201, MFR_CTRL1_reg_value);
}

//********************************MFR_CTRL2 REG********************************//


void get_mp4201_MFR_CTRL2_reg_info(hMP4201_t *mp4201)
{
    uint8_t MFR_CTRL2_REG_value = 0;
    MP4201_ReadByte(mp4201->Device_Address,MFR_CTRL2_COMMAND_CODE, &MFR_CTRL2_REG_value);

    mp4201->MFR_CTRL2.DISCHARGER_EN            = (MFR_CTRL2_REG_value & 0x20) >> 5;
    mp4201->MFR_CTRL2.Discharge_current_select = (MFR_CTRL2_REG_value & 0x18) >> 3;
    mp4201->MFR_CTRL2.Line_compensation_select = (MFR_CTRL2_REG_value & 0x07);
}

void write_mp4201_MFR_CTRL2_reg(hMP4201_t *mp4201, uint8_t MFR_CTRL2_reg_value)
{
    MP4201_WriteByte(mp4201->Device_Address,MFR_CTRL2_COMMAND_CODE,MFR_CTRL2_reg_value);
    get_mp4201_MFR_CTRL2_reg_info(mp4201);
}

void Reset_mp4201_MFR_CTRL2(hMP4201_t *mp4201)
{
    MP4201_WriteByte(mp4201->Device_Address,MFR_CTRL2_COMMAND_CODE,0x28);
    get_mp4201_MFR_CTRL2_reg_info(mp4201);
}

uint8_t transfer_mp4201_MFR_CTRL2_reg(const hMP4201_MFR_CTRL2_t *MFR_CTRL2)
{
    uint8_t MFR_CTRL2_reg_value = 0x28;
    MFR_CTRL2_reg_value = MFR_CTRL2_reg_value | MFR_CTRL2->DISCHARGER_EN << 5;
    MFR_CTRL2_reg_value = MFR_CTRL2_reg_value | MFR_CTRL2->Discharge_current_select << 3;
    MFR_CTRL2_reg_value = MFR_CTRL2_reg_value | MFR_CTRL2->Line_compensation_select;
    return MFR_CTRL2_reg_value;
}

void mp4201_discharger_enable_set(hMP4201_t *mp4201, bool status)
{
    uint8_t MFR_CTRL2_reg_value = 0x28;
    mp4201->MFR_CTRL2.DISCHARGER_EN = status;
    MFR_CTRL2_reg_value = transfer_mp4201_MFR_CTRL2_reg(&mp4201->MFR_CTRL2);
    write_mp4201_MFR_CTRL2_reg(mp4201, MFR_CTRL2_reg_value);
}

void mp4201_discharge_current_set(hMP4201_t *mp4201, hMP4201_Discharge_current_select_e discharge_current_value)
{
    uint8_t MFR_CTRL2_reg_value = 0x28;
    mp4201->MFR_CTRL2.Discharge_current_select = discharge_current_value;
    MFR_CTRL2_reg_value = transfer_mp4201_MFR_CTRL2_reg(&mp4201->MFR_CTRL2);
    write_mp4201_MFR_CTRL2_reg(mp4201, MFR_CTRL2_reg_value);
}

void mp4201_line_drop_compensation_set(hMP4201_t *mp4201, hMP4201_line_drop_compensation_e line_drop_compensation_value)
{
    uint8_t MFR_CTRL2_reg_value = 0x28;
    mp4201->MFR_CTRL2.Line_compensation_select = line_drop_compensation_value;
    MFR_CTRL2_reg_value = transfer_mp4201_MFR_CTRL2_reg(&mp4201->MFR_CTRL2);
    write_mp4201_MFR_CTRL2_reg(mp4201, MFR_CTRL2_reg_value);
}

//********************************MFR_OCP_CTRL REG********************************//

void get_mp4201_MFR_OCP_CTRL_reg_info(hMP4201_t *mp4201)
{
    uint8_t MFR_OCP_CTRL_REG_value = 0;
    MP4201_ReadByte(mp4201->Device_Address,MFR_OCP_CTRL_COMMAND_CODE, &MFR_OCP_CTRL_REG_value);

    mp4201->MFR_OCP_CTRL.Switching_current_limit_value = (MFR_OCP_CTRL_REG_value & 0xE0) >> 4;
    mp4201->MFR_OCP_CTRL.Rsense1_value                 = (MFR_OCP_CTRL_REG_value & 0x0C) >> 2;
    mp4201->MFR_OCP_CTRL.Rsense2_value                 = (MFR_OCP_CTRL_REG_value & 0x03);
}

/**
 * @brief 直接向 MP4201 的 MFR_OCP_CTRL 写入数据
 * @param mp4201 MP4201 I2C 地址
 * @param MFR_OCP_CTRL_reg_value MP4201_MFR_OCP_CTRL 8位寄存器数据
 */
void write_mp4201_MFR_OCP_CTRL_reg(hMP4201_t *mp4201, uint8_t MFR_OCP_CTRL_reg_value)
{
    MP4201_WriteByte(mp4201->Device_Address,MFR_OCP_CTRL_COMMAND_CODE,MFR_OCP_CTRL_reg_value);
    Reset_mp4201_MFR_OCP_CTRL(mp4201);
}

void Reset_mp4201_MFR_OCP_CTRL(hMP4201_t *mp4201)
{
    MP4201_WriteByte(mp4201->Device_Address,MFR_OCP_CTRL_COMMAND_CODE,0x60);
    get_mp4201_MFR_OCP_CTRL_reg_info(mp4201);
}

uint8_t transfer_mp4201_MFR_OCP_CTRL_reg(const hMP4201_MFR_OCP_CTRL_t *MFR_OCP_CTRL)
{
    uint8_t MFR_OCP_CTRL_reg_value = 0x60;
    MFR_OCP_CTRL_reg_value = MFR_OCP_CTRL_reg_value | MFR_OCP_CTRL->Switching_current_limit_value << 4;
    MFR_OCP_CTRL_reg_value = MFR_OCP_CTRL_reg_value | MFR_OCP_CTRL->Rsense1_value << 2;
    MFR_OCP_CTRL_reg_value = MFR_OCP_CTRL_reg_value | MFR_OCP_CTRL->Rsense2_value;
    return MFR_OCP_CTRL_reg_value;
}

void mp4201_current_limit_set(hMP4201_t *mp4201, hMP4201_switching_current_limit_e switching_current_value)
{
    uint8_t MFR_OCP_CTRL_reg_value = 0x60;
    mp4201->MFR_OCP_CTRL.Switching_current_limit_value = switching_current_value;
    MFR_OCP_CTRL_reg_value = transfer_mp4201_MFR_OCP_CTRL_reg(&mp4201->MFR_OCP_CTRL);
    write_mp4201_MFR_OCP_CTRL_reg(mp4201, MFR_OCP_CTRL_reg_value);
}

void mp4201_Rsense1_set(hMP4201_t *mp4201, hMP4201_Rsense_value_select_e Rsense1_value)
{
    uint8_t MFR_OCP_CTRL_reg_value = 0x60;
    mp4201->MFR_OCP_CTRL.Rsense1_value = Rsense1_value;
    MFR_OCP_CTRL_reg_value = transfer_mp4201_MFR_OCP_CTRL_reg(&mp4201->MFR_OCP_CTRL);
    write_mp4201_MFR_OCP_CTRL_reg(mp4201, MFR_OCP_CTRL_reg_value);
}

void mp4201_Rsense2_set(hMP4201_t *mp4201, hMP4201_Rsense_value_select_e Rsense2_value)
{
    uint8_t MFR_OCP_CTRL_reg_value = 0x60;
    mp4201->MFR_OCP_CTRL.Rsense2_value = Rsense2_value;
    MFR_OCP_CTRL_reg_value = transfer_mp4201_MFR_OCP_CTRL_reg(&mp4201->MFR_OCP_CTRL);
    write_mp4201_MFR_OCP_CTRL_reg(mp4201, MFR_OCP_CTRL_reg_value);
}

//********************************MFR_CTRL3 REG********************************//

void get_mp4201_MFR_CTRL3_reg_info(hMP4201_t *mp4201)
{
    uint8_t MFR_CTRL3_REG_value = 0;
    MP4201_ReadByte(mp4201->Device_Address,MFR_CTRL3_COMMAND_CODE, &MFR_CTRL3_REG_value);

    mp4201->MFR_CTRL3.OUT_CC_EN     = (MFR_CTRL3_REG_value & 0x80) >> 7;
    mp4201->MFR_CTRL3.IN_CC_EN      = (MFR_CTRL3_REG_value & 0x40) >> 6;
    mp4201->MFR_CTRL3.ADC_FORCED_ON = (MFR_CTRL3_REG_value & 0x20) >> 5;
    mp4201->MFR_CTRL3.GPIO_function = (MFR_CTRL3_REG_value & 0x18) >> 3;
    mp4201->MFR_CTRL3.I2C_Address   = (MFR_CTRL3_REG_value & 0x03);
}

static  hMP4201_I2C_Address_e transfer_MP4201_t_device_address(uint8_t addr)
{
    switch(addr)
    {
        case 0x27:
            return I2C_ADDRESS_27H;

        case 0x2F:
            return I2C_ADDRESS_2FH;

        case 0x37:
            return I2C_ADDRESS_37H;

        case 0x3F:
            return I2C_ADDRESS_3FH;

        default:
            break;
    }

    return  0;
}

/**
 * @brief 直接向 MP4201 的 MFR_CTRL3 写入数据
 * @param mp4201 MP4201 I2C 地址
 * @param MFR_CTRL3_reg_value MP4201_MFR_CTRL3 8位寄存器数据
 */
void write_mp4201_MFR_CTRL3_reg(hMP4201_t *mp4201, uint8_t MFR_CTRL3_reg_value)
{
    MP4201_WriteByte(mp4201->Device_Address,MFR_CTRL3_COMMAND_CODE,MFR_CTRL3_reg_value);
    get_mp4201_MFR_CTRL3_reg_info(mp4201);
}

// @todo 待测试
void Reset_mp4201_MFR_CTRL3(hMP4201_t *mp4201)
{
    MP4201_WriteByte(mp4201->Device_Address,MFR_CTRL3_COMMAND_CODE,0x87 | transfer_MP4201_t_device_address(mp4201->Device_Address));
    get_mp4201_MFR_CTRL3_reg_info(mp4201);
}

uint8_t transfer_mp4201_MFR_CTRL3_reg(const hMP4201_MFR_CTRL3_t *MFR_CTRL3)
{
    uint8_t MFR_CTRL3_reg_value = 0x87;
    MFR_CTRL3_reg_value = MFR_CTRL3_reg_value | MFR_CTRL3->OUT_CC_EN << 7;
    MFR_CTRL3_reg_value = MFR_CTRL3_reg_value | MFR_CTRL3->IN_CC_EN << 6;
    MFR_CTRL3_reg_value = MFR_CTRL3_reg_value | MFR_CTRL3->ADC_FORCED_ON << 5;
    MFR_CTRL3_reg_value = MFR_CTRL3_reg_value | MFR_CTRL3->GPIO_function << 3;
    MFR_CTRL3_reg_value = MFR_CTRL3_reg_value | MFR_CTRL3->I2C_Address;
    return MFR_CTRL3_reg_value;
}

void mp4201_OUT_CC_enable_set(hMP4201_t *mp4201, bool status)
{
    uint8_t MFR_CTRL3_reg_value = 0x87;
    mp4201->MFR_CTRL3.OUT_CC_EN = status;
    MFR_CTRL3_reg_value = transfer_mp4201_MFR_CTRL3_reg(&mp4201->MFR_CTRL3);
    write_mp4201_MFR_CTRL3_reg(mp4201, MFR_CTRL3_reg_value);
}

void mp4201_IN_CC_enable_set(hMP4201_t *mp4201, bool status)
{
    uint8_t MFR_CTRL3_reg_value = 0x87;
    mp4201->MFR_CTRL3.IN_CC_EN = status;
    MFR_CTRL3_reg_value = transfer_mp4201_MFR_CTRL3_reg(&mp4201->MFR_CTRL3);
    write_mp4201_MFR_CTRL3_reg(mp4201, MFR_CTRL3_reg_value);
}

/**
 * @brief 当Operation false时候，ADC采样开启/关闭
 * @param mp4201
 * @param status
 */

void mp4201_ADC_FORCED_enable_set(hMP4201_t *mp4201, bool status)
{
    uint8_t MFR_CTRL3_reg_value = 0x87;
    mp4201->MFR_CTRL3.ADC_FORCED_ON = status;
    MFR_CTRL3_reg_value = transfer_mp4201_MFR_CTRL3_reg(&mp4201->MFR_CTRL3);
    write_mp4201_MFR_CTRL3_reg(mp4201, MFR_CTRL3_reg_value);
}

void mp4201_GPIO_function_set(hMP4201_t *mp4201, hMP4201_GPIO_function_select_e  GPIO_function)
{
    uint8_t MFR_CTRL3_reg_value = 0x87;
    mp4201->MFR_CTRL3.GPIO_function = GPIO_function;
    MFR_CTRL3_reg_value = transfer_mp4201_MFR_CTRL3_reg(&mp4201->MFR_CTRL3);
    write_mp4201_MFR_CTRL3_reg(mp4201, MFR_CTRL3_reg_value);
}

/**
 * @brief 只更新寄存器中的值，并不更新结构体中IIC的值
 * @param mp4201
 * @param I2C_Address
 */
void mp4201_I2C_Address_set(hMP4201_t *mp4201, hMP4201_I2C_Address_e I2C_Address)
{
    uint8_t MFR_CTRL3_reg_value = 0x87;
    mp4201->MFR_CTRL3.I2C_Address = I2C_Address;
    MFR_CTRL3_reg_value = transfer_mp4201_MFR_CTRL3_reg(&mp4201->MFR_CTRL3);
    write_mp4201_MFR_CTRL3_reg(mp4201, MFR_CTRL3_reg_value);
}

//********************************MFR_CTRL4 REG********************************//

void get_mp4201_MFR_CTRL4_reg_info(hMP4201_t *mp4201)
{
    uint8_t MFR_CTRL4_REG_value = 0;
    MP4201_ReadByte(mp4201->Device_Address,MFR_CTRL4_COMMAND_CODE, &MFR_CTRL4_REG_value);

    mp4201->MFR_CTRL4.Battery_Charging_EN        = (MFR_CTRL4_REG_value & 0x80) >> 7;
    mp4201->MFR_CTRL4.Input_CC_blank_time_value  = (MFR_CTRL4_REG_value & 0x60) >> 5;
    mp4201->MFR_CTRL4.Pre_Charge_EN              = (MFR_CTRL4_REG_value & 0x10) >> 4;
    mp4201->MFR_CTRL4.Output_CC_blank_time_value = (MFR_CTRL4_REG_value & 0x0C) >> 2;
    mp4201->MFR_CTRL4.Precharge_voltage_value    = (MFR_CTRL4_REG_value & 0x02) >> 1;
    mp4201->MFR_CTRL4.EN_TERM_CHG                = (MFR_CTRL4_REG_value & 0x01);
}

/**
 * @brief 直接向 MP4201 的 MFR_CTRL4 写入数据
 * @param mp4201 MP4201 I2C 地址
 * @param MFR_CTRL4_reg_value MP4201_MFR_CTRL4 8位寄存器数据
 */
void write_mp4201_MFR_CTRL4_reg(hMP4201_t *mp4201, uint8_t MFR_CTRL4_reg_value)
{
    MP4201_WriteByte(mp4201->Device_Address,MFR_CTRL4_COMMAND_CODE,MFR_CTRL4_reg_value);
    get_mp4201_MFR_CTRL4_reg_info(mp4201);
}

void Reset_mp4201_MFR_CTRL4(hMP4201_t *mp4201)
{
    MP4201_WriteByte(mp4201->Device_Address,MFR_CTRL4_COMMAND_CODE,0x10);
    get_mp4201_MFR_CTRL4_reg_info(mp4201);
}

uint8_t transfer_mp4201_MFR_CTRL4_reg(const hMP4201_MFR_CTRL4_t *MFR_CTRL4)
{
    uint8_t MFR_CTRL4_reg_value = 0x10;
    MFR_CTRL4_reg_value = MFR_CTRL4_reg_value | MFR_CTRL4->Battery_Charging_EN << 7;
    MFR_CTRL4_reg_value = MFR_CTRL4_reg_value | MFR_CTRL4->Input_CC_blank_time_value  << 5;
    MFR_CTRL4_reg_value = MFR_CTRL4_reg_value | MFR_CTRL4->Pre_Charge_EN << 4;
    MFR_CTRL4_reg_value = MFR_CTRL4_reg_value | MFR_CTRL4->Output_CC_blank_time_value << 2;
    MFR_CTRL4_reg_value = MFR_CTRL4_reg_value | MFR_CTRL4->Precharge_voltage_value << 1;
    MFR_CTRL4_reg_value = MFR_CTRL4_reg_value | MFR_CTRL4->EN_TERM_CHG;
    return MFR_CTRL4_reg_value;
}

void mp4201_Battery_Charging_enable_set(hMP4201_t *mp4201, bool status)
{
    uint8_t MFR_CTRL4_reg_value = 0x10;
    mp4201->MFR_CTRL4.Battery_Charging_EN = status;
    MFR_CTRL4_reg_value = transfer_mp4201_MFR_CTRL4_reg(&mp4201->MFR_CTRL4);
    write_mp4201_MFR_CTRL4_reg(mp4201, MFR_CTRL4_reg_value);
}

void mp4201_input_CC_black_time_set(hMP4201_t *mp4201, hMP4201_CC_BLANK_TIMER_e CC_blank_timer)
{
    uint8_t MFR_CTRL4_reg_value = 0x10;
    mp4201->MFR_CTRL4.Input_CC_blank_time_value = CC_blank_timer;
    MFR_CTRL4_reg_value = transfer_mp4201_MFR_CTRL4_reg(&mp4201->MFR_CTRL4);
    write_mp4201_MFR_CTRL4_reg(mp4201, MFR_CTRL4_reg_value);
}

void mp4201_Pre_Charge_enable_set(hMP4201_t *mp4201, bool status)
{
    uint8_t MFR_CTRL4_reg_value = 0x10;
    mp4201->MFR_CTRL4.Pre_Charge_EN = status;
    MFR_CTRL4_reg_value = transfer_mp4201_MFR_CTRL4_reg(&mp4201->MFR_CTRL4);
    write_mp4201_MFR_CTRL4_reg(mp4201, MFR_CTRL4_reg_value);
}

void mp4201_output_CC_black_time_set(hMP4201_t *mp4201, hMP4201_CC_BLANK_TIMER_e CC_blank_timer)
{
    uint8_t MFR_CTRL4_reg_value = 0x10;
    mp4201->MFR_CTRL4.Output_CC_blank_time_value = CC_blank_timer;
    MFR_CTRL4_reg_value = transfer_mp4201_MFR_CTRL4_reg(&mp4201->MFR_CTRL4);
    write_mp4201_MFR_CTRL4_reg(mp4201, MFR_CTRL4_reg_value);
}

void mp4201_EN_TERM_CHG_set(hMP4201_t *mp4201, bool status)
{
    uint8_t MFR_CTRL4_reg_value = 0x10;
    mp4201->MFR_CTRL4.EN_TERM_CHG = status;
    MFR_CTRL4_reg_value = transfer_mp4201_MFR_CTRL4_reg(&mp4201->MFR_CTRL4);
    write_mp4201_MFR_CTRL4_reg(mp4201, MFR_CTRL4_reg_value);
}

//********************************MFR_CTRL5 REG********************************//

void get_mp4201_MFR_CTRL5_reg_info(hMP4201_t *mp4201)
{
    uint8_t MFR_CTRL5_REG_value = 0;
    MP4201_ReadByte(mp4201->Device_Address,MFR_CTRL5_COMMAND_CODE, &MFR_CTRL5_REG_value);

    mp4201->MFR_CTRL5.Slew_rate_value           = (MFR_CTRL5_REG_value & 0xC0) >> 6;
    mp4201->MFR_CTRL5.Slew_fall_value           = (MFR_CTRL5_REG_value & 0x30) >> 4;
    mp4201->MFR_CTRL5.Termination_current_value = (MFR_CTRL5_REG_value & 0x0F);
}

/**
 * @brief 直接向 MP4201 的 MFR_CTRL5 写入数据
 * @param mp4201
 * @param MFR_CTRL5_reg_value MP4201_MFR_CTRL5 8位寄存器数据
 */
void write_mp4201_MFR_CTRL5_reg(hMP4201_t *mp4201, uint8_t MFR_CTRL5_reg_value)
{
    MP4201_WriteByte(mp4201->Device_Address,MFR_CTRL5_COMMAND_CODE,MFR_CTRL5_reg_value);
    get_mp4201_MFR_CTRL5_reg_info(mp4201);
}

void Reset_mp4201_MFR_CTRL5(hMP4201_t *mp4201)
{
    MP4201_WriteByte(mp4201->Device_Address,MFR_CTRL5_COMMAND_CODE,0x61);
    get_mp4201_MFR_CTRL5_reg_info(mp4201);
}

uint8_t transfer_mp4201_MFR_CTRL5_reg(const hMP4201_MFR_CTRL5_t *MFR_CTRL5)
{
    uint8_t MFR_CTRL5_reg_value = 0x61;
    MFR_CTRL5_reg_value = MFR_CTRL5_reg_value | MFR_CTRL5->Slew_rate_value  << 6;
    MFR_CTRL5_reg_value = MFR_CTRL5_reg_value | MFR_CTRL5->Slew_fall_value << 4;
    MFR_CTRL5_reg_value = MFR_CTRL5_reg_value | MFR_CTRL5->Termination_current_value;
    return MFR_CTRL5_reg_value;
}

void mp4201_slew_rate_set(hMP4201_t *mp4201, hMP4201_slew_rate_e slew_rate_value)
{
    uint8_t MFR_CTRL5_reg_value = 0x61;
    mp4201->MFR_CTRL5.Slew_rate_value = slew_rate_value;
    MFR_CTRL5_reg_value = transfer_mp4201_MFR_CTRL5_reg(&mp4201->MFR_CTRL5);
    write_mp4201_MFR_CTRL5_reg(mp4201, MFR_CTRL5_reg_value);
}

void mp4201_slew_fall_set(hMP4201_t *mp4201, hMP4201_slew_fall_e slew_fall_value)
{
    uint8_t MFR_CTRL5_reg_value = 0x61;
    mp4201->MFR_CTRL5.Slew_fall_value = slew_fall_value;
    MFR_CTRL5_reg_value = transfer_mp4201_MFR_CTRL5_reg(&mp4201->MFR_CTRL5);
    write_mp4201_MFR_CTRL5_reg(mp4201, MFR_CTRL5_reg_value);
}

void mp4201_termination_current_set(hMP4201_t *mp4201, hMP4201_termination_current_e Termination_current_value)
{
    uint8_t MFR_CTRL5_reg_value = 0x61;
    mp4201->MFR_CTRL5.Termination_current_value = Termination_current_value;
    MFR_CTRL5_reg_value = transfer_mp4201_MFR_CTRL5_reg(&mp4201->MFR_CTRL5);
    write_mp4201_MFR_CTRL5_reg(mp4201, MFR_CTRL5_reg_value);
}

//********************************MFR_OVP_CTRL REG********************************//

void get_mp4201_MFR_OVP_CTRL_reg_info(hMP4201_t *mp4201)
{
    uint8_t MFR_OVP_CTRL_REG_value = 0;
    MP4201_ReadByte(mp4201->Device_Address,MFR_OVP_CTRL_COMMAND_CODE, &MFR_OVP_CTRL_REG_value);

    mp4201->MFR_OVP_CTRL.VIN_REG_EN         = (MFR_OVP_CTRL_REG_value & 0x80) >> 7;
    mp4201->MFR_OVP_CTRL.ABS_VOUT_OVP_value = (MFR_OVP_CTRL_REG_value & 0x70) >> 4;
    mp4201->MFR_OVP_CTRL.VOUT_OVP_EN        = (MFR_OVP_CTRL_REG_value & 0x08) >> 3;
    mp4201->MFR_OVP_CTRL.VIN_OVP_value      = (MFR_OVP_CTRL_REG_value & 0x03);
}

/**
 * @brief 直接向 MP4201 的 MFR_OVP_CTRL 写入数据
 * @param mp4201 MP4201 I2C 地址
 * @param MFR_OVP_CTRL_reg_value MP4201_MFR_OVP_CTRL 8位寄存器数据
 */
void write_mp4201_MFR_OVP_CTRL_reg(hMP4201_t *mp4201, uint8_t MFR_OVP_CTRL_reg_value)
{
    MP4201_WriteByte(mp4201->Device_Address,MFR_OVP_CTRL_COMMAND_CODE,MFR_OVP_CTRL_reg_value);
    get_mp4201_MFR_OVP_CTRL_reg_info(mp4201);
}

void Reset_mp4201_MFR_OVP_CTRL(hMP4201_t *mp4201)
{
    MP4201_WriteByte(mp4201->Device_Address,MFR_OVP_CTRL_COMMAND_CODE,0x10);
    get_mp4201_MFR_OVP_CTRL_reg_info(mp4201);
}

uint8_t transfer_mp4201_MFR_OVP_CTRL_reg(const hMP4201_MFR_OVP_CTRL_t *MFR_OVP_CTRL)
{
    uint8_t MFR_OVP_CTRL_reg_value = 0x61;
    MFR_OVP_CTRL_reg_value = MFR_OVP_CTRL_reg_value | MFR_OVP_CTRL->VIN_REG_EN << 7;
    MFR_OVP_CTRL_reg_value = MFR_OVP_CTRL_reg_value | MFR_OVP_CTRL->ABS_VOUT_OVP_value << 4;
    MFR_OVP_CTRL_reg_value = MFR_OVP_CTRL_reg_value | MFR_OVP_CTRL->VOUT_OVP_EN << 3;
    MFR_OVP_CTRL_reg_value = MFR_OVP_CTRL_reg_value | MFR_OVP_CTRL->VIN_OVP_value;
    return MFR_OVP_CTRL_reg_value;
}

void mp4201_VIN_REG_enable_set(hMP4201_t *mp4201, bool status)
{
    uint8_t MFR_OVP_CTRL_reg_value = 0x61;
    mp4201->MFR_OVP_CTRL.VIN_REG_EN = status;
    MFR_OVP_CTRL_reg_value = transfer_mp4201_MFR_OVP_CTRL_reg(&mp4201->MFR_OVP_CTRL);
    write_mp4201_MFR_OVP_CTRL_reg(mp4201, MFR_OVP_CTRL_reg_value);
}

void mp4201_abs_vout_OVP_set(hMP4201_t *mp4201, hMP4201_ABS_VOUT_OVP_threshold_e abs_vout_OVP_value)
{
    uint8_t MFR_OVP_CTRL_reg_value = 0x61;
    mp4201->MFR_OVP_CTRL.ABS_VOUT_OVP_value = abs_vout_OVP_value;
    MFR_OVP_CTRL_reg_value = transfer_mp4201_MFR_OVP_CTRL_reg(&mp4201->MFR_OVP_CTRL);
    write_mp4201_MFR_OVP_CTRL_reg(mp4201, MFR_OVP_CTRL_reg_value);
}

void mp4201_VOUT_OVP_enable_set(hMP4201_t *mp4201, bool status)
{
    uint8_t MFR_OVP_CTRL_reg_value = 0x61;
    mp4201->MFR_OVP_CTRL.VOUT_OVP_EN = status;
    MFR_OVP_CTRL_reg_value = transfer_mp4201_MFR_OVP_CTRL_reg(&mp4201->MFR_OVP_CTRL);
    write_mp4201_MFR_OVP_CTRL_reg(mp4201, MFR_OVP_CTRL_reg_value);
}

void mp4201_vin_OVP_set(hMP4201_t *mp4201, hMP4201_VIN_OVP_threshold_e vin_OVP_value)
{
    uint8_t MFR_OVP_CTRL_reg_value = 0x61;
    mp4201->MFR_OVP_CTRL.VOUT_OVP_EN = vin_OVP_value;
    MFR_OVP_CTRL_reg_value = transfer_mp4201_MFR_OVP_CTRL_reg(&mp4201->MFR_OVP_CTRL);
    write_mp4201_MFR_OVP_CTRL_reg(mp4201, MFR_OVP_CTRL_reg_value);
}


//********************************MFR_PRE_CURRENT REG********************************//

/**
 * @brief MP4201电池预充电电流选择
 * @param mp4201 MP4201 I2C 地址
 * @param charge_current_value 电池预充电电流 范围 0.1A-6.4A, 50mA/step, 默认0x10, 800mA
 */
void mp4201_battery_pre_charge_current_set(hMP4201_t *mp4201, float charge_current_value)
{
    if (charge_current_value > 6.4f)
        charge_current_value = 6.4f;
    else if (charge_current_value < 0.1f)
        charge_current_value = 0.1f;
    uint8_t charge_current_value_reg_val = 0x10;
    charge_current_value_reg_val = (uint8_t)(charge_current_value/50);
    MP4201_WriteByte(mp4201->Device_Address, MFR_PRE_CURRENT_COMMAND_CODE, charge_current_value_reg_val);
}

void mp4201_read_battery_pre_charge_current_set(hMP4201_t *mp4201)
{
    uint8_t charge_current_value_reg_val = 0x10;
    MP4201_ReadByte(mp4201->Device_Address, MFR_PRE_CURRENT_COMMAND_CODE, &charge_current_value_reg_val);
    mp4201->precharge_current_value = (float)(charge_current_value_reg_val*0.05);
}

void Reset_mp4201_battery_pre_charge_current(hMP4201_t *mp4201)
{
    uint8_t charge_current_value_reg_val = 0x10;
    MP4201_ReadByte(mp4201->Device_Address, MFR_PRE_CURRENT_COMMAND_CODE, &charge_current_value_reg_val);
    mp4201_read_battery_pre_charge_current_set(mp4201);
}

//********************************MFR_STATUS_MASK********************************//

void get_mp4201_MFR_STATUS_MASK_reg_info(hMP4201_t *mp4201)
{
    uint8_t MFR_STATUS_MASK_REG_value = 0;
    MP4201_ReadByte(mp4201->Device_Address,MFR_STATUS_MASK_COMMAND_CODE, &MFR_STATUS_MASK_REG_value);

    mp4201->MFR_STATUS_MASK.VOUT_MASK             = (MFR_STATUS_MASK_REG_value & 0x80) >> 7;
    mp4201->MFR_STATUS_MASK.SCP_MASK              = (MFR_STATUS_MASK_REG_value & 0x40) >> 6;
    mp4201->MFR_STATUS_MASK.CRC_MASK              = (MFR_STATUS_MASK_REG_value & 0x20) >> 5;
    mp4201->MFR_STATUS_MASK.TEMPERATURE_MASK      = (MFR_STATUS_MASK_REG_value & 0x10) >> 4;
    mp4201->MFR_STATUS_MASK.PG_STATUS_MASK        = (MFR_STATUS_MASK_REG_value & 0x08) >> 3;
    mp4201->MFR_STATUS_MASK.CHARGER_COMPLETE_MASK = (MFR_STATUS_MASK_REG_value & 0x04) >> 2;
    mp4201->MFR_STATUS_MASK.VIN_MASK              = (MFR_STATUS_MASK_REG_value & 0x02) >> 1;
    mp4201->MFR_STATUS_MASK.IIN_MASK              = (MFR_STATUS_MASK_REG_value & 0x01);
}

/**
 * @brief 直接向 MP4201 的 MFR_STATUS_MASK 写入数据
 * @param mp4201
 * @param MFR_STATUS_MASK_reg_value MP4201_MFR_STATUS_MASK 8位寄存器数据
 * @note  屏蔽异常事件，即标记对应异常事见发生时不下拉ALT引脚，但是状态寄存器仍然会改变
 */
void write_mp4201_MFR_STATUS_MASK_reg(hMP4201_t *mp4201, uint8_t MFR_STATUS_MASK_reg_value)
{
    MP4201_WriteByte(mp4201->Device_Address,MFR_STATUS_MASK_COMMAND_CODE,MFR_STATUS_MASK_reg_value);
    get_mp4201_MFR_STATUS_MASK_reg_info(mp4201);
}

void Reset_mp4201_MFR_STATUS_MASK(hMP4201_t *mp4201)
{
    MP4201_WriteByte(mp4201->Device_Address,MFR_STATUS_MASK_COMMAND_CODE,0xBF);
    get_mp4201_MFR_STATUS_MASK_reg_info(mp4201);
}

uint8_t transfer_mp4201_MFR_STATUS_MASK_reg(const hMP4201_MFR_STATUS_MASK_t *MFR_STATUS_MASK)
{
    uint8_t MFR_STATUS_MASK_reg_value = 0xBF;

    MFR_STATUS_MASK_reg_value = MFR_STATUS_MASK_reg_value | MFR_STATUS_MASK->VOUT_MASK << 7;
    MFR_STATUS_MASK_reg_value = MFR_STATUS_MASK_reg_value | MFR_STATUS_MASK->SCP_MASK << 6;
    MFR_STATUS_MASK_reg_value = MFR_STATUS_MASK_reg_value | MFR_STATUS_MASK->CRC_MASK << 5;
    MFR_STATUS_MASK_reg_value = MFR_STATUS_MASK_reg_value | MFR_STATUS_MASK->TEMPERATURE_MASK << 4;
    MFR_STATUS_MASK_reg_value = MFR_STATUS_MASK_reg_value | MFR_STATUS_MASK->PG_STATUS_MASK << 3;
    MFR_STATUS_MASK_reg_value = MFR_STATUS_MASK_reg_value | MFR_STATUS_MASK->CHARGER_COMPLETE_MASK << 2;
    MFR_STATUS_MASK_reg_value = MFR_STATUS_MASK_reg_value | MFR_STATUS_MASK->VIN_MASK << 1;
    MFR_STATUS_MASK_reg_value = MFR_STATUS_MASK_reg_value | MFR_STATUS_MASK->IIN_MASK;
    return MFR_STATUS_MASK_reg_value;
}

void mp4201_vout_mask_set(hMP4201_t *mp4201, bool status)
{
    uint8_t MFR_STATUS_MASK_reg_value = 0xBF;
    mp4201->MFR_STATUS_MASK.VOUT_MASK = status;
    MFR_STATUS_MASK_reg_value = transfer_mp4201_MFR_STATUS_MASK_reg(&mp4201->MFR_STATUS_MASK);
    write_mp4201_MFR_STATUS_MASK_reg(mp4201, MFR_STATUS_MASK_reg_value);
}

void mp4201_iout_or_pout_mask_mask_set(hMP4201_t *mp4201, bool status)
{
    uint8_t MFR_STATUS_MASK_reg_value = 0xBF;
    mp4201->MFR_STATUS_MASK.SCP_MASK = status;
    MFR_STATUS_MASK_reg_value = transfer_mp4201_MFR_STATUS_MASK_reg(&mp4201->MFR_STATUS_MASK);
    write_mp4201_MFR_STATUS_MASK_reg(mp4201, MFR_STATUS_MASK_reg_value);
}

void mp4201_CRC_mask_set(hMP4201_t *mp4201, bool status)
{
    uint8_t MFR_STATUS_MASK_reg_value = 0xBF;
    mp4201->MFR_STATUS_MASK.CRC_MASK = status;
    MFR_STATUS_MASK_reg_value = transfer_mp4201_MFR_STATUS_MASK_reg(&mp4201->MFR_STATUS_MASK);
    write_mp4201_MFR_STATUS_MASK_reg(mp4201, MFR_STATUS_MASK_reg_value);
}

void mp4201_temperature_mask_set(hMP4201_t *mp4201, bool status)
{
    uint8_t MFR_STATUS_MASK_reg_value = 0xBF;
    mp4201->MFR_STATUS_MASK.TEMPERATURE_MASK = status;
    MFR_STATUS_MASK_reg_value = transfer_mp4201_MFR_STATUS_MASK_reg(&mp4201->MFR_STATUS_MASK);
    write_mp4201_MFR_STATUS_MASK_reg(mp4201, MFR_STATUS_MASK_reg_value);
}

void mp4201_PG_status_mask_set(hMP4201_t *mp4201, bool status)
{
    uint8_t MFR_STATUS_MASK_reg_value = 0xBF;
    mp4201->MFR_STATUS_MASK.PG_STATUS_MASK = status;
    MFR_STATUS_MASK_reg_value = transfer_mp4201_MFR_STATUS_MASK_reg(&mp4201->MFR_STATUS_MASK);
    write_mp4201_MFR_STATUS_MASK_reg(mp4201, MFR_STATUS_MASK_reg_value);
}

void mp4201_charger_complete_mask_set(hMP4201_t *mp4201, bool status)
{
    uint8_t MFR_STATUS_MASK_reg_value = 0xBF;
    mp4201->MFR_STATUS_MASK.CHARGER_COMPLETE_MASK = status;
    MFR_STATUS_MASK_reg_value = transfer_mp4201_MFR_STATUS_MASK_reg(&mp4201->MFR_STATUS_MASK);
    write_mp4201_MFR_STATUS_MASK_reg(mp4201, MFR_STATUS_MASK_reg_value);
}

void mp4201_vin_mask_set(hMP4201_t *mp4201, bool status)
{
    uint8_t MFR_STATUS_MASK_reg_value = 0xBF;
    mp4201->MFR_STATUS_MASK.VIN_MASK = status;
    MFR_STATUS_MASK_reg_value = transfer_mp4201_MFR_STATUS_MASK_reg(&mp4201->MFR_STATUS_MASK);
    write_mp4201_MFR_STATUS_MASK_reg(mp4201, MFR_STATUS_MASK_reg_value);
}

void mp4201_iin_mask_set(hMP4201_t *mp4201, bool status)
{
    uint8_t MFR_STATUS_MASK_reg_value = 0xBF;
    mp4201->MFR_STATUS_MASK.IIN_MASK = status;
    MFR_STATUS_MASK_reg_value = transfer_mp4201_MFR_STATUS_MASK_reg(&mp4201->MFR_STATUS_MASK);
    write_mp4201_MFR_STATUS_MASK_reg(mp4201, MFR_STATUS_MASK_reg_value);
}


//********************************MFR_OTP_CONFIGURATION_CODE REG********************************//

void get_MFR_OTP_configuration_code_info(hMP4201_t *mp4201)
{
    uint8_t data = 0;
    MP4201_ReadByte(mp4201->Device_Address,MFR_OTP_CONFIGURATION_CODE_COMMAND_CODE, &data);
    mp4201->MFR_OTP_configuration_code = data;
}


//********************************MFR_OTP_REVISION_NUMBER REG********************************//

void get_MFR_OTP_revision_number_info(hMP4201_t *mp4201)
{
    uint8_t data = 0;
    MP4201_ReadByte(mp4201->Device_Address,MFR_OTP_CONFIGURATION_CODE_COMMAND_CODE, &data);
    mp4201->MFR_OTP_revision_number = data;
}

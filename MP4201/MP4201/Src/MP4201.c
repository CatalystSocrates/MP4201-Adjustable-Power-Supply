/********************************************************************************
* @author: LTS
* @date: 25-7-19 下午1:52
* @version: 1.0
* @description:
********************************************************************************/

#include "MP4201.h"

hMP4201_t MP4201;

void get_mp4021_all_MFR_CTRL_info(hMP4201_t *mp4201)
{
    get_mp4201_MFR_CTRL1_reg_info(mp4201);
    get_mp4201_MFR_CTRL2_reg_info(mp4201);
    get_mp4201_MFR_CTRL3_reg_info(mp4201);
    get_mp4201_MFR_CTRL4_reg_info(mp4201);
    get_mp4201_MFR_CTRL5_reg_info(mp4201);

    get_mp4201_MFR_OCP_CTRL_reg_info(mp4201);
    get_mp4201_MFR_OVP_CTRL_reg_info(mp4201);
}

void get_mp4201_OTP_info(hMP4201_t *mp4201)
{
    get_MFR_OTP_configuration_code_info(mp4201);
    get_MFR_OTP_revision_number_info(mp4201);
}

/**
 * @brief 获取MP4201的所有读数信息
 * @param mp4201 mp4201 结构体指针
 * @param count 读取次数
 */
void get_mp4201_all_read_data_info(hMP4201_t *mp4201, uint8_t count)
{
	float vin_average         = 0.0f;
	float iin_average         = 0.0f;
	float vout_average        = 0.0f;
	float iout_average        = 0.0f;
	float temperature_average = 0.0f;

	for (int i = 0; i < count; i++)
	{
		uint16_t vin         = 0;
		uint16_t iin         = 0;
		uint16_t vout        = 0;
		uint16_t iout        = 0;
		uint16_t temperature = 0;

		MP4201_ReadWord(mp4201->Device_Address,READ_VIN_COMMAND_CODE, &vin);
		MP4201_ReadWord(mp4201->Device_Address,READ_IIN_COMMAND_CODE, &iin);
		MP4201_ReadWord(mp4201->Device_Address,READ_VOUT_COMMAND_CODE, &vout);
		MP4201_ReadWord(mp4201->Device_Address,READ_IOUT_COMMAND_CODE, &iout);
		MP4201_ReadWord(mp4201->Device_Address,READ_TEMPERATURE_COMMAND_CODE, &temperature);

		vin_average 		  += (float)(vin & 0x3FF) * 0.0832f;
		iin_average    		+= (float)(iin & 0x3FF) * 0.043f;
		vout_average 	    += (float)(vout & 0x3FF) * 0.0832f;
		iout_average 	    += (float)(iout & 0x3FF) * 0.043f;
		temperature_average += -272.48f + 1.07f * (float)temperature;

		HAL_Delay(2);
	}

	vin_average         /= (float)count;
	iin_average         /= (float)count;
	vout_average        /= (float)count;
	iout_average 	    /= (float)count;
	temperature_average /= (float)count;

	mp4201->read_info.Vin_read  	   = vin_average;
	mp4201->read_info.Iin_read  	   = iin_average;
	mp4201->read_info.Vout_read 	   = vout_average;
	mp4201->read_info.Iout_read 	   = iout_average;
	mp4201->read_info.Temperature_read = temperature_average;
	
}

void get_mp4201_all_info(hMP4201_t *mp4201)
{
    get_mp4021_all_MFR_CTRL_info(mp4201);
    mp4201_read_operation_status(mp4201);
    mp4201_read_vout_set(mp4201);
    mp4201_read_vin_reg_thld_set(mp4201);
    mp4201_read_iout_oc_fault_limit_set(mp4201);
    mp4201_read_iin_oc_fault_limit_set(mp4201);
    mp4201_read_battery_pre_charge_current_set(mp4201);
    get_mp4201_status_info(mp4201);
    get_mp4201_status_temperature_info(mp4201);
    get_mp4201_all_read_data_info(mp4201, 5);
    get_mp4201_OTP_info(mp4201);
	  get_mp4201_MFR_STATUS_MASK_reg_info(mp4201);
}


/**
 * @brief 重置所有控制寄存器，会将输出关闭！！！
 * @param mp4201
 */
void mp4201_reset_all_MRF_CTRL_reg(hMP4201_t *mp4201)
{
    mp4201_operation_set(mp4201,false);

    Reset_mp4201_MFR_CTRL1(mp4201);
    Reset_mp4201_MFR_CTRL2(mp4201);
    Reset_mp4201_MFR_CTRL3(mp4201);
    Reset_mp4201_MFR_CTRL4(mp4201);
    Reset_mp4201_MFR_CTRL5(mp4201);

    Reset_mp4201_MFR_OVP_CTRL(mp4201);
    Reset_mp4201_MFR_OCP_CTRL(mp4201);

    //验证更新信息
    get_mp4021_all_MFR_CTRL_info(mp4201);
}
// // @todo 需要更新写入内部flash的程序
// void Is_init_first_mp4201(hMP4201_t *mp4201)
// {
//
// }

/**
 * @brief 配置MP4201功能
 * @param mp4201
 */
void MP4201_Init(hMP4201_t *mp4201)
{
    mp4201->Device_Address = MP4201_ADDR;
	  HAL_Delay(200); // MP4201上电后需要等一段时间通信，不然容易出现一些难以理解的问题
	  mp4201_operation_set(&MP4201,false);
		Reset_mp4201_MFR_CTRL1(mp4201);
		get_mp4201_all_info(mp4201);
		mp4201_clear_faults(mp4201);
		mp4201_VOUT_OVP_enable_set(mp4201,0x00);
//		mp4201_DIR_set(&MP4201,1);
//		mp4201_vin_OVP_set(mp4201,VIN_OVP_60V);
//		mp4201_abs_vout_OVP_set(mp4201,ABS_VOUT_OVP_104);
		mp4201_DEAD_Time_set(mp4201,DEAD_TIME_20NS);
//    mp4201_vin_reg_thld_set(mp4201, 48.0f);
//    mp4201_OCP_Mode_set(mp4201, HICCUP_MODE);
		mp4201_VOUT_OVP_enable_set(mp4201,0x00);
	  mp4201_FB_Mode_set(mp4201, INTERNAL_FB);
		get_mp4201_all_info(mp4201);
    mp4201_operation_set(mp4201,true);
}

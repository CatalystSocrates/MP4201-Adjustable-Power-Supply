/********************************************************************************
* @author: LTS
* @date: 25-7-19 下午1:52
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef MP4201DRIVER_H
#define MP4201DRIVER_H

#include "MP4201Middleware.h"

#define MP4201_INTERNAL_FEEDBACK_RATIO 32.0f

extern uint8_t MFR_CTRL1_REG_value;
extern uint8_t MFR_CTRL1_reg_value;

typedef struct {
    // status_word_H
    bool SCP_fault;
    bool Vin_overvoltage_fault;
    bool PG_STATUS;
    bool Is_Charger_completed;
    bool Iin_overcurrent_fault;

    // status_word_L
    bool Vout_overvoltage_fault;
    bool Iout_overcurrent_fault;
    bool Temperature_fault;
    bool CRC_error_fault;
}hMP4201_status_info_t;

typedef struct {
    bool OTP_fault;
    bool OTP_warning_fault;
    bool NTC_fault;
}hMP4201_status_temperature_t;

//********************************MFR_CTRL1 REG********************************//

typedef enum {
    MINIMUN_UVLO              = 0x00,
    Rising_Voltage_8_POINT_5V = 0x01,
    Rising_Voltage_11V        = 0x02,
    Rising_Voltage_20V        = 0x03,
}hMP4201_Input_UVLO_e;

typedef enum {
    EXTERNAL_FB = 0x00,
    INTERNAL_FB = 0x01,
}hMP4201_FB_MODE_e;

typedef enum {
    HICCUP_MODE   = 0x00,
    CONSTANT_MODE = 0x01,
}hMP4201_OCP_MODE_e;

typedef enum {
    DEAD_TIME_15NS = 0x00,
    DEAD_TIME_20NS = 0x01,
    DEAD_TIME_25NS = 0x02,
    DEAD_TIME_30NS = 0x03,
}hMP4201_DEAD_TIME_e;

typedef struct {
    bool                 DIR_CONFIG;     // 0: current flow from out to in; 1: current flow from in to out
    hMP4201_Input_UVLO_e Input_UVLO_SEL; // 00: minimum UVLO;  01: 8.5V Rising Voltage;  10: 22V Rising Voltage;  11: 40V Rising Voltage;
    bool                 GPIO1_GATE_EN;  // 0: GATE Pin disable;  1: GATE Pin enable
    hMP4201_FB_MODE_e    FB_MODE;
    hMP4201_OCP_MODE_e   OCP_MODE;
    hMP4201_DEAD_TIME_e  DEAD_TIME;
}hMP4201_MFR_CTRL1_t;


//********************************MFR_CTRL2 REG********************************//

typedef enum {
    DISCHARGER_25mA  = 0x00,
    DISCHARGER_50mA  = 0x01,
    DISCHARGER_75mA  = 0x02,
    DISCHARGER_100mA = 0x03,
}hMP4201_Discharge_current_select_e;

typedef enum {
    COMPENSATION_NONE        = 0x00,
    COMPENSATION_80mV_5AOUT  = 0x01,
    COMPENSATION_160mV_5AOUT = 0x02,
    COMPENSATION_240mV_5AOUT = 0x03,
    COMPENSATION_360mV_5AOUT = 0x04,
    COMPENSATION_480mV_5AOUT = 0x05,
    COMPENSATION_600mV_5AOUT = 0x06,
    COMPENSATION_720mV_5AOUT = 0x07,
}hMP4201_line_drop_compensation_e; // 大电流电压补偿
// @todo 需要增加外置fb的函数
typedef struct {
    bool DISCHARGER_EN;       // 0: disable;  1: enable
    hMP4201_Discharge_current_select_e Discharge_current_select;
    hMP4201_line_drop_compensation_e   Line_compensation_select;
}hMP4201_MFR_CTRL2_t;


//********************************MFR_OCP_CTRL REG********************************//

typedef enum {
    PEAK_CURRENT_10A_VALLEY_CURRENT_8A_LIMIT  = 0x00,
    PEAK_CURRENT_15A_VALLEY_CURRENT_12A_LIMIT = 0x01,
    PEAK_CURRENT_20A_VALLEY_CURRENT_17A_LIMIT = 0x02,
    PEAK_CURRENT_25A_VALLEY_CURRENT_22A_LIMIT = 0x03,
    PEAK_CURRENT_30A_VALLEY_CURRENT_26A_LIMIT = 0x04,
    PEAK_CURRENT_35A_VALLEY_CURRENT_30A_LIMIT = 0x05,
}hMP4201_switching_current_limit_e; // 峰值谷值电流限制

typedef enum {
    RESISTANCE_5mOHM  = 0x00,
    RESISTANCE_2mOHM  = 0x01,
    // RESISTANCE_1mOHM  = 0x02,
}hMP4201_Rsense_value_select_e; // 单位：毫欧

typedef struct {
    hMP4201_switching_current_limit_e Switching_current_limit_value;
    hMP4201_Rsense_value_select_e     Rsense1_value;
    hMP4201_Rsense_value_select_e     Rsense2_value;
}hMP4201_MFR_OCP_CTRL_t;


//********************************MFR_OCP_CTRL REG********************************//

typedef enum {
    PG_INDICATION = 0x00,
    EXTERNAL_CLOCK_SYNCHRONIZATION = 0x01,
    BATTERY_CHARGING_COMPLETE_INDICATION = 0x02,
}hMP4201_GPIO_function_select_e;

typedef enum {
    I2C_ADDRESS_27H = 0x00,
    I2C_ADDRESS_2FH = 0x01,
    I2C_ADDRESS_37H = 0x02,
    I2C_ADDRESS_3FH = 0x03,
}hMP4201_I2C_Address_e;

typedef struct {
    bool OUT_CC_EN;           // Output average current control
    bool IN_CC_EN;            // Input average current control
    bool ADC_FORCED_ON;       // ADC function enable/disable when set I2C Operation off
    hMP4201_GPIO_function_select_e  GPIO_function;
    hMP4201_I2C_Address_e           I2C_Address;
}hMP4201_MFR_CTRL3_t;


//********************************MFR_CTRL4 REG********************************//

typedef enum {
    BLANK_TIMER_400uS = 0x00,
    BLANK_TIMER_4mS   = 0x01,
    BLANK_TIMER_16mS  = 0x02,
    BLANK_TIMER_64mS  = 0x03,
}hMP4201_CC_BLANK_TIMER_e;

typedef enum {
    PRECHARGE_VOLTAGE_in_60_percent_Vout = 0x00,
    PRECHARGE_VOLTAGE_in_70_percent_Vout = 0x01,
}hMP4201_precharge_voltage_e;

typedef struct {
    bool EN_TERM_CHG;         // 0: the charge will not be disabled when I_CHG < I_TERM  1: the charge will be disabled and charge is terminated when I_CHG < I_TERM
    bool Pre_Charge_EN;
    bool Battery_Charging_EN; // 0: the battery charge function is disabled including pre-charge and termination detection
                              // 1: the battery charge function is enabled including pre-charge, CC charge and termination detection
    hMP4201_CC_BLANK_TIMER_e    Input_CC_blank_time_value;
    hMP4201_CC_BLANK_TIMER_e    Output_CC_blank_time_value;
    hMP4201_precharge_voltage_e Precharge_voltage_value;
}hMP4201_MFR_CTRL4_t;


//********************************MFR_CTRL4 REG********************************//

typedef enum {
    SLEW_RATE_RISE_80uV_1uS  = 0x00,
    SLEW_RATE_RISE_160uV_1uS = 0x01,
    SLEW_RATE_RISE_400uV_1uS = 0x02,
    SLEW_RATE_RISE_800uV_1uS = 0x03,
}hMP4201_slew_rate_e; // 单位uV/ms

typedef enum {
    SLEW_RATE_FALL_5uV_1uS   = 0x00,
    SLEW_RATE_FALL_40uV_1uS  = 0x01,
    SLEW_RATE_FALL_100uV_1uS = 0x02,
    SLEW_RATE_FALL_200uV_1uS = 0x03,
}hMP4201_slew_fall_e;

typedef enum {
    TERMINATION_CURRENT_400mA_200mA_1step = 0x01,
}hMP4201_termination_current_e; // 400mA, 200mA/step

typedef struct {
    hMP4201_slew_rate_e           Slew_rate_value;
    hMP4201_slew_fall_e           Slew_fall_value;
    hMP4201_termination_current_e Termination_current_value;
}hMP4201_MFR_CTRL5_t;
// @todo 需要增加外置fb的函数

//********************************MFR_OVP_CTRL REG********************************//

typedef enum {
    ABS_VOUT_OVP_30_POINT_8 = 0x00,
    ABS_VOUT_OVP_39_POINT_6 = 0x01,
    ABS_VOUT_OVP_52_POINT_8 = 0x02,
    ABS_VOUT_OVP_62         = 0x03,
    ABS_VOUT_OVP_84         = 0x04,
    ABS_VOUT_OVP_104        = 0x05,
}hMP4201_ABS_VOUT_OVP_threshold_e;

typedef enum {
    VIN_OVP_84V     = 0x00,
    VIN_OVP_30V     = 0x01,
    VIN_OVP_60V     = 0x02,
    VIN_OVP_104V    = 0x03,
}hMP4201_VIN_OVP_threshold_e;

typedef enum {
    PFM_WO_FSS    = 0,
    PFM_W_FSS     = 1365,
    FCCM_WO_FSS   = 2730,
    FCCM_W_FSS    = 4096,
}MP4201_MODE;

typedef enum {
    FREQ_200KHz     = 0,
    FREQ_400KHz     = 1365,
    FREQ_600KHz     = 2730,
    FREQ_1MHz       = 4095,
}MP4201_FREQ;

typedef struct {
    bool VIN_REG_EN;
    bool VOUT_OVP_EN;
    hMP4201_VIN_OVP_threshold_e      VIN_OVP_value;
    hMP4201_ABS_VOUT_OVP_threshold_e ABS_VOUT_OVP_value;
}hMP4201_MFR_OVP_CTRL_t;



//********************************MFR_PRE_CURRENT REG********************************//

typedef struct {
    bool VOUT_MASK;
    bool SCP_MASK;
    bool CRC_MASK;
    bool TEMPERATURE_MASK;
    bool PG_STATUS_MASK;
    bool CHARGER_COMPLETE_MASK;
    bool VIN_MASK;
    bool IIN_MASK;
}hMP4201_MFR_STATUS_MASK_t;




typedef struct {
    float Vin_read;
    float Vout_read;
    float Iin_read;
    float Iout_read;
    float Temperature_read;
		float P_in;
		float P_out;
	  float Eff;
}hMP4201_read_info_t;

#pragma pack(1)
typedef struct {
    bool operation;

    float Vin_THLD;
    float Vout_set;
    float I_output_overcurrent_limit_value;
    float I_input_overcurrent_limit_value;
    float precharge_current_value;
    float external_feedback_ratio;
	  
		uint16_t vout_reg_val;

    hMP4201_read_info_t read_info;
    hMP4201_status_info_t status_info;
    hMP4201_status_temperature_t temperature_info;
    hMP4201_MFR_CTRL1_t MFR_CTRL1;
    hMP4201_MFR_CTRL2_t MFR_CTRL2;
    hMP4201_MFR_CTRL3_t MFR_CTRL3;
    hMP4201_MFR_CTRL4_t MFR_CTRL4;
    hMP4201_MFR_CTRL5_t MFR_CTRL5;

    hMP4201_MFR_OVP_CTRL_t MFR_OVP_CTRL;
    hMP4201_MFR_OCP_CTRL_t MFR_OCP_CTRL;
    hMP4201_MFR_STATUS_MASK_t MFR_STATUS_MASK;
    hMP4201_precharge_voltage_e precharge_voltage;

    uint8_t MFR_OTP_configuration_code;
    uint8_t MFR_OTP_revision_number;

    uint8_t Device_Address;
}hMP4201_t;
#pragma pack()

extern hMP4201_t MP4201;

void mp4201_operation_set(hMP4201_t *mp4201, bool status);;
void mp4201_read_operation_status(hMP4201_t *mp4201);;


void mp4201_vout_set(hMP4201_t *mp4201, float vout);;
void mp4201_read_vout_set(hMP4201_t *mp4201);


void mp4201_vin_reg_thld_set(hMP4201_t *mp4201, float vin_reg_thld_value);
void mp4201_read_vin_reg_thld_set(hMP4201_t *mp4201);


void mp4201_iout_oc_fault_limit_set(hMP4201_t *mp4201, float iout_oc_fault_limit_value);
void mp4201_read_iout_oc_fault_limit_set(hMP4201_t *mp4201);


void mp4201_iin_oc_fault_limit_set(hMP4201_t *mp4201, float iin_oc_fault_limit_value);
void mp4201_read_iin_oc_fault_limit_set(hMP4201_t *mp4201);


void get_mp4201_status_info(hMP4201_t *mp4201);


void mp4201_clear_faults(hMP4201_t *mp4201);


void get_mp4201_status_temperature_info(hMP4201_t *mp4201);


void get_mp4201_vin_info(hMP4201_t *mp4201);
void get_mp4201_average_vin_info(hMP4201_t *mp4201, uint8_t count);
void get_mp4201_iin_info(hMP4201_t *mp4201);
void get_mp4201_average_iin_info(hMP4201_t *mp4201, uint8_t count);
void get_mp4201_vout_info(hMP4201_t *mp4201);
void get_mp4201_average_vout_info(hMP4201_t *mp4201, uint8_t count);
void get_mp4201_iout_info(hMP4201_t *mp4201);
uint16_t get_mp4201_iout_reg_info(hMP4201_t *mp4201);
void get_mp4201_average_iout_info(hMP4201_t *mp4201, uint8_t count);
void get_mp4201_temperature_info(hMP4201_t *mp4201);
void get_mp4201_average_temperature_info(hMP4201_t *mp4201, uint8_t count);


void get_mp4201_MFR_CTRL1_reg_info(hMP4201_t *mp4201);
void write_mp4201_MFR_CTRL1_reg(hMP4201_t *mp4201, uint8_t MFR_CTRL1_reg_value);
void Reset_mp4201_MFR_CTRL1(hMP4201_t *mp4201);
uint8_t transfer_mp4201_MFR_CTRL1_reg(const hMP4201_MFR_CTRL1_t *MFR_CTRL1);
void mp4201_DIR_set(hMP4201_t *mp4201, bool dir);
void mp4201_Input_UVLO_set(hMP4201_t *mp4201, hMP4201_Input_UVLO_e input_uvlo_sel);
void mp4201_GPIO1_GATE_enable_set(hMP4201_t *mp4201, bool status);
void mp4201_FB_Mode_set(hMP4201_t *mp4201, hMP4201_FB_MODE_e fb_mode);
void mp4201_OCP_Mode_set(hMP4201_t *mp4201, hMP4201_OCP_MODE_e ocp_mode);
void mp4201_DEAD_Time_set(hMP4201_t *mp4201, hMP4201_DEAD_TIME_e dead_time);

void get_mp4201_MFR_CTRL2_reg_info(hMP4201_t *mp4201);
void write_mp4201_MFR_CTRL2_reg(hMP4201_t *mp4201, uint8_t MFR_CTRL2_reg_value);
void Reset_mp4201_MFR_CTRL2(hMP4201_t *mp4201);
uint8_t transfer_mp4201_MFR_CTRL2_reg(const hMP4201_MFR_CTRL2_t *MFR_CTRL2);
void mp4201_discharger_enable_set(hMP4201_t *mp4201, bool status);
void mp4201_discharge_current_set(hMP4201_t *mp4201, hMP4201_Discharge_current_select_e discharge_current_value);
void mp4201_line_drop_compensation_set(hMP4201_t *mp4201, hMP4201_line_drop_compensation_e line_drop_compensation_value);
void get_mp4201_MFR_OCP_CTRL_reg_info(hMP4201_t *mp4201);
void write_mp4201_MFR_OCP_CTRL_reg(hMP4201_t *mp4201, uint8_t MFR_OCP_CTRL_reg_value);
void Reset_mp4201_MFR_OCP_CTRL(hMP4201_t *mp4201);
uint8_t transfer_mp4201_MFR_OCP_CTRL_reg(const hMP4201_MFR_OCP_CTRL_t *MFR_OCP_CTRL);
void mp4201_current_limit_set(hMP4201_t *mp4201, hMP4201_switching_current_limit_e switching_current_value);
void mp4201_Rsense1_set(hMP4201_t *mp4201, hMP4201_Rsense_value_select_e Rsense1_value);
void mp4201_Rsense2_set(hMP4201_t *mp4201, hMP4201_Rsense_value_select_e Rsense2_value);


void get_mp4201_MFR_CTRL3_reg_info(hMP4201_t *mp4201);
void write_mp4201_MFR_CTRL3_reg(hMP4201_t *mp4201, uint8_t MFR_CTRL3_reg_value);
void Reset_mp4201_MFR_CTRL3(hMP4201_t *mp4201);
uint8_t transfer_mp4201_MFR_CTRL3_reg(const hMP4201_MFR_CTRL3_t *MFR_CTRL3);
void mp4201_OUT_CC_enable_set(hMP4201_t *mp4201, bool status);
void mp4201_IN_CC_enable_set(hMP4201_t *mp4201, bool status);
void mp4201_ADC_FORCED_enable_set(hMP4201_t *mp4201, bool status);
void mp4201_GPIO_function_set(hMP4201_t *mp4201, hMP4201_GPIO_function_select_e  GPIO_function);
void mp4201_I2C_Address_set(hMP4201_t *mp4201, hMP4201_I2C_Address_e I2C_Address);


void get_mp4201_MFR_CTRL4_reg_info(hMP4201_t *mp4201);
void write_mp4201_MFR_CTRL4_reg(hMP4201_t *mp4201, uint8_t MFR_CTRL4_reg_value);
void Reset_mp4201_MFR_CTRL4(hMP4201_t *mp4201);
uint8_t transfer_mp4201_MFR_CTRL4_reg(const hMP4201_MFR_CTRL4_t *MFR_CTRL4);
void mp4201_Battery_Charging_enable_set(hMP4201_t *mp4201, bool status);
void mp4201_input_CC_black_time_set(hMP4201_t *mp4201, hMP4201_CC_BLANK_TIMER_e CC_blank_timer);
void mp4201_Pre_Charge_enable_set(hMP4201_t *mp4201, bool status);
void mp4201_output_CC_black_time_set(hMP4201_t *mp4201, hMP4201_CC_BLANK_TIMER_e CC_blank_timer);
void mp4201_EN_TERM_CHG_set(hMP4201_t *mp4201, bool status);


void get_mp4201_MFR_CTRL5_reg_info(hMP4201_t *mp4201);
void write_mp4201_MFR_CTRL5_reg(hMP4201_t *mp4201, uint8_t MFR_CTRL5_reg_value);
void Reset_mp4201_MFR_CTRL5(hMP4201_t *mp4201);
uint8_t transfer_mp4201_MFR_CTRL5_reg(const hMP4201_MFR_CTRL5_t *MFR_CTRL5);
void mp4201_slew_rate_set(hMP4201_t *mp4201, hMP4201_slew_rate_e slew_rate_value);
void mp4201_slew_fall_set(hMP4201_t *mp4201, hMP4201_slew_fall_e slew_fall_value);
void mp4201_termination_current_set(hMP4201_t *mp4201, hMP4201_termination_current_e Termination_current_value);


void get_mp4201_MFR_OVP_CTRL_reg_info(hMP4201_t *mp4201);
void write_mp4201_MFR_OVP_CTRL_reg(hMP4201_t *mp4201, uint8_t MFR_OVP_CTRL_reg_value);
void Reset_mp4201_MFR_OVP_CTRL(hMP4201_t *mp4201);
uint8_t transfer_mp4201_MFR_OVP_CTRL_reg(const hMP4201_MFR_OVP_CTRL_t *MFR_OVP_CTRL);
void mp4201_VIN_REG_enable_set(hMP4201_t *mp4201, bool status);
void mp4201_abs_vout_OVP_set(hMP4201_t *mp4201, hMP4201_ABS_VOUT_OVP_threshold_e abs_vout_OVP_value);
void mp4201_VOUT_OVP_enable_set(hMP4201_t *mp4201, bool status);
void mp4201_vin_OVP_set(hMP4201_t *mp4201, hMP4201_VIN_OVP_threshold_e vin_OVP_value);


void mp4201_battery_pre_charge_current_set(hMP4201_t *mp4201, float charge_current_value);
void mp4201_read_battery_pre_charge_current_set(hMP4201_t *mp4201);
void Reset_mp4201_battery_pre_charge_current(hMP4201_t *mp4201);


void get_mp4201_MFR_STATUS_MASK_reg_info(hMP4201_t *mp4201);
void write_mp4201_MFR_STATUS_MASK_reg(hMP4201_t *mp4201, uint8_t MFR_STATUS_MASK_reg_value);
void Reset_mp4201_MFR_STATUS_MASK(hMP4201_t *mp4201);
uint8_t transfer_mp4201_MFR_STATUS_MASK_reg(const hMP4201_MFR_STATUS_MASK_t *MFR_STATUS_MASK);
void mp4201_vout_mask_set(hMP4201_t *mp4201, bool status);
void mp4201_iout_or_pout_mask_mask_set(hMP4201_t *mp4201, bool status);
void mp4201_CRC_mask_set(hMP4201_t *mp4201, bool status);
void mp4201_temperature_mask_set(hMP4201_t *mp4201, bool status);
void mp4201_PG_status_mask_set(hMP4201_t *mp4201, bool status);
void mp4201_charger_complete_mask_set(hMP4201_t *mp4201, bool status);
void mp4201_vin_mask_set(hMP4201_t *mp4201, bool status);
void mp4201_iin_mask_set(hMP4201_t *mp4201, bool status);

void get_MFR_OTP_configuration_code_info(hMP4201_t *mp4201);
void get_MFR_OTP_revision_number_info(hMP4201_t *mp4201);

#endif //MP4201DRIVER_H

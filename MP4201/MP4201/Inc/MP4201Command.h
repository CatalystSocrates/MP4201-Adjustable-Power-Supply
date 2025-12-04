/********************************************************************************
* @autor: LTS
* @date: 25-7-19 下午2:07
* @version: 1.0
* @description: MP4201 指令
********************************************************************************/
#ifndef MP4201COMMAND_CODE_H
#define MP4201COMMAND_CODE_H

// OPREATION COMMAND_CODE
#define OPERATION_COMMAND_CODE                   0x01
#define CLEAR_FAULTS_COMMAND_CODE                0x03
#define VOUT_COMMAND_COMMAND_CODE                0x21

// LIMIT COMMAND_CODE
#define IOUT_OC_FAULT_LIMIT_COMMAND_CODE         0x46
#define VIN_REG_THLD_COMMAND_CODE                0x59
#define IIN_OC_FAULT_LIMIT_COMMAND_CODE          0x5B
#define STATUS_WORD_COMMAND_CODE                 0x79
#define STATUS_TEMPER_ATURE_COMMAND_CODE         0x7D

// READ DATA COMMAND_CODE
#define READ_VIN_COMMAND_CODE                    0x88
#define READ_IIN_COMMAND_CODE                    0x89
#define READ_VOUT_COMMAND_CODE                   0x8B
#define READ_IOUT_COMMAND_CODE                   0x8C
#define READ_TEMPERATURE_COMMAND_CODE            0x8D

// MFR COMMAND_CODE
#define MFR_CTRL1_COMMAND_CODE                   0xD0
#define MFR_CTRL2_COMMAND_CODE                   0xD1
#define MFR_OCP_CTRL_COMMAND_CODE                0xD2
#define MFR_CTRL3_COMMAND_CODE                   0xD3
#define MFR_CTRL4_COMMAND_CODE                   0xD4
#define MFR_CTRL5_COMMAND_CODE                   0xD5
#define MFR_OVP_CTRL_COMMAND_CODE                0xD6
#define MFR_PRE_CURRENT_COMMAND_CODE             0xD7
#define MFR_STATUS_MASK_COMMAND_CODE             0xD8
#define MFR_OTP_CONFIGURATION_CODE_COMMAND_CODE  0xD9
#define MFR_OTP_REVISION_NUMBER_COMMAND_CODE     0xDA

#endif //MP4201COMMAND_CODE_H

/********************************************************************************
* @author: LTS
* @date: 25-7-19 下午1:52
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef MP4201_H
#define MP4201_H

#include "MP4201Driver.h"
#include "MP4201Command.h"
#include "MP4201Middleware.h"

#define VIN_THLD_INIT_VALUE 48.0f
#define VOUT_INIT_VALUE     24.0f
#define EXTERNAL_FEEDBACK_RATIO 5.0f

#define I_INPUT_OVERCURRENT_LIMIT_INIT_VALUE  20.0f
#define I_OUTPUT_OVERCURRENT_LIMIT_INIT_VALUE 5.0f
#define PRE_CURRENT_INIT_VALUE 0.8f

void get_mp4021_all_MFR_CTRL_info(hMP4201_t *mp4201);
void get_mp4201_OTP_info(hMP4201_t *mp4201);
void get_mp4201_all_read_data_info(hMP4201_t *mp4201, uint8_t count);
void get_mp4201_all_info(hMP4201_t *mp4201);
void mp4201_reset_all_MRF_CTRL_reg(hMP4201_t *mp4201);
void MP4201_Init(hMP4201_t *mp4201);



#endif //MP4201_H

#include "screen.h"
#include "string.h"
#include "MP4201Driver.h"
#include "MCP4725.h"
float SETV;
float SETI;
float temperature;
float Vin;
float Iin;
float Pin;
float Vout;
float Iout;
float Pout;
float Eff;
bool OPEN;
bool CV_CC;
int mode;
int freq;
int i;
extern float R_Iin;
extern float R_Iout;

void yprintf(const char * ystring)
{
	uint16_t str_len = strlen(ystring);
	HAL_UART_Transmit(&huart1, (uint8_t*)ystring, str_len,10);
}


void dataupdate()
{
	if(uart1_recv_frame[0]==0xAA)
	{
		vout_target=uart1_recv_frame[1] + uart1_recv_frame[2]*0.01f;
		SETI=uart1_recv_frame[3] + uart1_recv_frame[4]*0.01f;

		mp4201_vout_set(&MP4201, vout_target);
		mp4201_iout_oc_fault_limit_set(&MP4201,SETI);
	}else if(uart1_recv_frame[0]==0xEE)
	{
		OPEN=uart1_recv_frame[1];
		mp4201_operation_set(&MP4201,OPEN);
	}else if(uart1_recv_frame[0]==0xCC)
	{
		CV_CC=uart1_recv_frame[1];
	}else if(uart1_recv_frame[0]==0xDE)
	{
		mode=uart1_recv_frame[1];
		switch(mode)
		{
			case 1:
				MP4201_SET_MODE(PFM_WO_FSS);
				break;
			case 2:
				MP4201_SET_MODE(PFM_W_FSS);
				break;
			case 3:
				MP4201_SET_MODE(FCCM_WO_FSS);
				break;
			case 4:
				MP4201_SET_MODE(FCCM_W_FSS);
				break;
		}
	}else if(uart1_recv_frame[0]==0xFE)
	{
		freq=uart1_recv_frame[1];
		switch(freq){
			case 1:
				MP4201_SET_FREQ(FREQ_200KHz);
				break;
			case 2:
				MP4201_SET_FREQ(FREQ_400KHz);
				break;
			case 3:
				MP4201_SET_FREQ(FREQ_600KHz);
				break;
			case 4:
				MP4201_SET_FREQ(FREQ_1MHz);
				break;
		}
	}else if(uart1_recv_frame[0]==0xAB && uart1_recv_frame[1]==0xBA)
	{
		mode=1;
		freq=1;
		SETV=12.00f;
		SETI=4.00f;//屏幕上电初始化后，保持主控和屏幕信息一致
	}

	memset(uart1_recv_frame,0,sizeof(uart1_recv_frame));
}
void xprintf(char * xstring,float xdata)
{
	// 定义缓冲区，50字节足够容纳绝大多数格式化场景（可根据需求调整）
    char message[50];
    // 1. 格式化字符串：将a和b拼接成目标字符串
    // snprintf避免缓冲区溢出，第二个参数为缓冲区总大小
    int len = snprintf(message, sizeof(message) - 3, xstring, xdata);
    // 减3是为预留3个0xFF的空间，防止溢出

    // 2. 仅当格式化成功时，追加3个0xFF并发送
    if (len > 0 && len < sizeof(message) - 3)
    {
        message[len]     = 0xFF;  // 第一个0xFF
        message[len + 1] = 0xFF;  // 第二个0xFF
        message[len + 2] = 0xFF;  // 第三个0xFF

        // 3. 串口发送：长度=格式化字符串长度+3个0xFF
        HAL_UART_Transmit(&huart1, (uint8_t*)message, len + 3 , 100);
    }
}
void screen_update()
{
  Vin = MP4201.read_info.Vin_read;    
	Iin = R_Iin;
	Pin = MP4201.read_info.P_in;
	Vout = MP4201.read_info.Vout_read;	   
	Iout = R_Iout;  
	Pout = MP4201.read_info.P_out;
	temperature = MP4201.read_info.Temperature_read;
	Eff = MP4201.read_info.Eff;
	if(MP4201.operation== 0)
	{
		Vout = 0.0f;	   
		Iout = 0.0f;  
		Pout = 0.0f;
	}
	xprintf("main.t0.txt=\"%.2f\"",Vout);
	xprintf("main.t1.txt=\"%.2f\"\xff\xff\xff",Iout);
	if(Pout>=100.0)
	{
		xprintf("main.t2.txt=\"%.1f\"\xff\xff\xff",Pout);
	}else xprintf("main.t2.txt=\"%.2f\"\xff\xff\xff",Pout);
	xprintf("main.t3.txt=\"%.1f\"\xff\xff\xff",temperature);
	xprintf("main.t6.txt=\"%.2f\"\xff\xff\xff",Vin);
	xprintf("main.t13.txt=\"%.2f\"\xff\xff\xff",Iin);
	if(Pin>=100.0)
	{
		xprintf("main.t22.txt=\"%.1f\"\xff\xff\xff",Pin);
	}else xprintf("main.t22.txt=\"%.2f\"\xff\xff\xff",Pin);
	xprintf("main.t26.txt=\"%.2f\"\xff\xff\xff",Eff);
	printf("add s0.id,0,%d\xff\xff\xff",(int)(Vout*3.125));
  printf("add s0.id,1,%d\xff\xff\xff",(int)(R_Iout*10));
	if(Iout < SETI + 0.5f && Iout > SETI - 0.5f)//若OCP处于使能状态，说明处于CC(恒流状态)
	{
		yprintf("CV_CC=1\xff\xff\xff");
	}else yprintf("CV_CC=0\xff\xff\xff");
}


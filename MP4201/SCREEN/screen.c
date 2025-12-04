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
float OVP;
float OCP;
float Eff;
bool OPEN;
bool CV_CC;//0ΪCV1ΪCC
int mode;
int freq;
int i;
void dataupdate()
{
	if(uart1_recv_frame[0]==0xAA)
	{
		vout_target=uart1_recv_frame[1] + uart1_recv_frame[2]*0.01;
		SETI=uart1_recv_frame[3] + uart1_recv_frame[4]*0.01;
		OVP=uart1_recv_frame[5] + uart1_recv_frame[6]*0.01;
		OCP=uart1_recv_frame[7] + uart1_recv_frame[8]*0.01;
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
		SETV=5.00;
		SETI=4.00;//屏幕上电初始化后，保持主控和屏幕信息一致
	}

	memset(uart1_recv_frame,0,sizeof(uart1_recv_frame));
}
void screen_update()
{
  Vin = MP4201.read_info.Vin_read;    
	Iin = MP4201.read_info.Iin_read;
	Pin = MP4201.read_info.P_in;
	Vout = MP4201.read_info.Vout_read;	   
	Iout = MP4201.read_info.Iout_read;  
	Pout = MP4201.read_info.P_out;
	temperature = MP4201.read_info.Temperature_read;
	Eff = MP4201.read_info.Eff;
	printf("main.t0.txt=\"%.2f\"\xff\xff\xff",Vout);
	printf("main.t1.txt=\"%.2f\"\xff\xff\xff",Iout);
	if(Pout>=100.0)
	{
		printf("main.t2.txt=\"%.1f\"\xff\xff\xff",Pout);
	}else printf("main.t2.txt=\"%.2f\"\xff\xff\xff",Pout);
	printf("main.t3.txt=\"%.1f\"\xff\xff\xff",temperature);
	printf("main.t6.txt=\"%.2f\"\xff\xff\xff",Vin);
	printf("main.t13.txt=\"%.2f\"\xff\xff\xff",Iin);
	printf("main.t22.txt=\"%.2f\"\xff\xff\xff",Pin);
	printf("main.t26.txt=\"%.2f\"\xff\xff\xff",Eff);
	printf("add s0.id,0,%d\xff\xff\xff",(int)((Vout-vout_target)*200));
	printf("add s0.id,1,%d\xff\xff\xff",(int)(Iout*1000)+128);
	
	if(fault_flag)
	{
		HAL_Delay(10);
		get_mp4201_status_info(&MP4201);
		if(MP4201.status_info.SCP_fault)printf("SCP_fault=1\xff\xff\xff"); 
		if(MP4201.status_info.Vin_overvoltage_fault)printf("Vin_overvoltage_fault=1\xff\xff\xff");
		//if(MP4201.status_info.PG_STATUS)printf("PG_STATUS=1\xff\xff\xff");
		if(MP4201.status_info.Is_Charger_completed)printf("Is_Charger_completed=1\xff\xff\xff");
		if(MP4201.status_info.Iin_overcurrent_fault)printf("Iin_overcurrent_fault=1\xff\xff\xff");
		if(MP4201.status_info.Vout_overvoltage_fault)printf("Vout_overvoltage_fault=1\xff\xff\xff");
		//if(MP4201.status_info.Iout_overcurrent_fault)printf("Iout_overcurrent_fault=1\xff\xff\xff");
		if(MP4201.status_info.Temperature_fault)printf("Temperature_fault=1\xff\xff\xff");
		if(MP4201.status_info.CRC_error_fault)printf("CRC_error_fault=1\xff\xff\xff");
		mp4201_clear_faults(&MP4201);
		fault_flag=0;
	}
	if(Iout < SETI + 0.5f && Iout > SETI - 0.5f)//若OCP处于使能状态，说明处于CC(恒流状态)
	{
		printf("CV_CC=1\xff\xff\xff");
	}else printf("CV_CC=0\xff\xff\xff");
}


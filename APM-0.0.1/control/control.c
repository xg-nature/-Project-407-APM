#include "control.h"
#include "sample_time.h"

/******************************/
//需要用到的头文件
//#include "datascope.h"
#include "straight_pid.h"
#include "encoder.h"


/******************************/

/**************采样时间变量******************/
unsigned char Time_Flag_2ms=0;      //采样时间标志位
unsigned char Time_Flag_5ms=0;
//unsigned char Time_Flag_10ms=0;

/********************************************/

/*******************************/
//其他文件引用过来的变量
short Encoder1_Speed;
extern u16 ringbuffer[1024];
extern u32 set,get;

/*******************************/





#if	SAMPLE_TIME==5
	void TIM5_IRQHandler(){
		if(TIM5->SR&0X0001)
		{
			TIM5->SR &= ~(1<<0);           //清除中断标志位
			
			Time_Flag_Inc();
			
			if(SAMPLE_FLAG_5MS == Time_Flag_5ms){
				int moto1_pwm=0;
				//5ms控制周期
				Time_Flag_5ms = 0;
				
				Encoder1_Speed = Get_Encoder1_Speed();
				moto1_pwm =SeqIntPID(4.8,0, 6,4000,Encoder1_Speed);
				Set_Pwm(moto1_pwm);
			}
			
			
			
			
			if(SAMPLE_FLAG_2MS == Time_Flag_2ms){
				ringbuffer[set%1024]=Get_Encoder1Val();
				set++;
				ringbuffer[set%1024]=Get_Encoder2Val();
				set++;
				ringbuffer[set%1024]=Get_Encoder3Val();
				set++;				
			}
			

		}
	}
	
#endif
	
void Time_Flag_Inc(void){
	//采样时间标志位自增
	
	Time_Flag_2ms++;
	Time_Flag_5ms++;
	
}

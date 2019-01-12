#include "straight_pid.h"

/**********************************/
//需要用到的头文件
#include "encode.h"
#include "pwm.h"
#include <math.h>
/**********************************/

/**********************************/
//需要定义的变量
extern u16	Parameter;

float ResultValue;
float ResultValueBack;
float ResultValueBackOne;
float ResultValueBackTwo;
float GiveValueBack;
float ResultValuess;
float PIDErrADD;
float ErrBack;
float SeqIntPIDErrADD;
float SeqIntErrBack;
float OverIntPIDErrADD;
float OverIntResultBack;
float OverIntErrBack;






float SeqIntPID(float Kp, float Ki, float Kd, float GiveValue, float ActualValue)
{
float result;
float Err,KpWork, KiWork, KdWork;
Err = GiveValue - ActualValue;
	if(fabs(Err) > 2*Parameter)
		Err=4*Parameter-fabs(Err);
KpWork = Kp*Err;
KiWork = Ki*SeqIntPIDErrADD;
KdWork = Kd*(Err-SeqIntErrBack);
if(fabs(Err) > 500)
{
result = KpWork+KdWork;
}else{
result = KpWork+KiWork+KdWork;
}
SeqIntPIDErrADD = SeqIntPIDErrADD + Err;
SeqIntErrBack = Err;
return result;
}


/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int motor_a)
{
	    if(motor_a>8400)
		motor_a=8400;
		else if (motor_a<-8400)
		motor_a=-8400;
    	if(motor_a>0)			
			INA=0;
		else  	       
			INA=1;
	    PWMA=motor_a;		
}


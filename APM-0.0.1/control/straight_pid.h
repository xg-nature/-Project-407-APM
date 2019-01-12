#ifndef	__STRAIGHT_PID_H
#define __STRAIGHT_PID_H



void Set_Pwm(int motor_a);
float SeqIntPID(float Kp, float Ki, float Kd, float GiveValue, float ActualValue);


#endif

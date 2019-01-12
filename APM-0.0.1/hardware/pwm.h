#ifndef	__PWM_H
#define __PWM_H

#include "stm32f4xx.h" 
#include "sys.h"

/*******************PWM**********************/
#define PWM
#define PWM_AHBxPeriph_GPIOx	RCC_AHB1Periph_GPIOA
#define PWM_APBxPeriph_TIMx		RCC_APB1Periph_TIM2
#define PWM_GPIOx 				GPIOA
#define PWM_AF_TIMx				GPIO_AF_TIM2
#define PWM_TIMx                TIM2
/********************************************/

/*******************PWM1*********************/
#define PWM1
#define PWM1_PinSource			GPIO_PinSource0
#define PWM1_Pin				GPIO_Pin_0
/********************************************/

/*******************PWM2*********************/
//#define PWM2
#define PWM2_PinSource			GPIO_PinSource1
#define PWM2_Pin				GPIO_Pin_1
/********************************************/

/*******************PWM3*********************/
//#define PWM3
#define PWM3_PinSource			GPIO_PinSource2
#define PWM3_Pin				GPIO_Pin_2
/********************************************/

/*******************PWM4*********************/
//#define PWM4
#define PWM4_PinSource			GPIO_PinSource3
#define PWM4_Pin				GPIO_Pin_3
/********************************************/


#define PWMD   PWM_TIMx->CCR4  
#define PWMC   PWM_TIMx->CCR3  
#define PWMB   PWM_TIMx->CCR2 
#define PWMA   PWM_TIMx->CCR1


#define IND   PAout(12)  
#define INC   PAout(11)  
#define INB   PDout(15)  
#define INA   PDout(14)

void PWM_Init(u32 arr, u32 psc);

#endif

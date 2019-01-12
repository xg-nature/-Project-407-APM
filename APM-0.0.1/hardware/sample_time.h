#ifndef	__SAMPLE_TIME_H
#define __SAMPLE_TIME_H

/*****************************
定时器文件
功能：定时采样
*****************************/
#include "stm32f4xx.h" 

/*********************SYSTEM_TIME************************/
//TIM5作为采样时钟 ：定时1ms
#define SAMPLE_TIME						5
#define	SAMPLE_TIME_APBxPeriph_TIMx		RCC_APB1Periph_TIM5
#define SAMPLE_TIME_TIMx				TIM5
#define SAMPLE_TIME_IRQn				TIM5_IRQn

/********************************************************/

void Sample_Time_Init(void);

#endif

#include "sample_time.h"

void Sample_Time_Init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure ;
	NVIC_InitTypeDef     NVIC_InitStructure ;
	
	/*打开时钟源*/
	RCC_APB1PeriphClockCmd(SAMPLE_TIME_APBxPeriph_TIMx,ENABLE);
	
	/*结构体配置*/
	TIM_TimeBaseInitStructure.TIM_Period = 4;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 8399;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(SAMPLE_TIME_TIMx,&TIM_TimeBaseInitStructure);
	
	/*开启定时器中断*/
	TIM_ITConfig(SAMPLE_TIME_TIMx,TIM_IT_Update,ENABLE);
	
	/*配置定时器中断*/
	NVIC_InitStructure.NVIC_IRQChannel = SAMPLE_TIME_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	/*使能定时器x*/
	TIM_Cmd(SAMPLE_TIME_TIMx,ENABLE);
}

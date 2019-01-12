#include "pwm.h"

/*PWM模式初始化*/
void PWM_Init(u32 arr, u32 psc)
{
	GPIO_InitTypeDef          GPIO_InitStructure ;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStructure ;
	TIM_OCInitTypeDef         TIM_OCInitStructure ;
  
  /*打开相应时钟源*/	
	RCC_AHB1PeriphClockCmd(PWM_AHBxPeriph_GPIOx,ENABLE);
	RCC_APB1PeriphClockCmd(PWM_APBxPeriph_TIMx,ENABLE);
  
	#ifdef	PWM1
		GPIO_PinAFConfig(PWM_GPIOx,PWM1_PinSource,PWM_AF_TIMx);
		/*配置电机输出的GPIO口*/
		GPIO_InitStructure.GPIO_Pin = PWM1_Pin;
		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF;                      //复用功能             
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                     //推挽复用输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                       //上拉
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                             
		GPIO_Init(PWM_GPIOx,&GPIO_InitStructure);
	#endif
	
	#ifdef PWM2
		GPIO_PinAFConfig(PWM_GPIOx,PWM2_PinSource,PWM_AF_TIMx);
		/*配置电机输出的GPIO口*/
		GPIO_InitStructure.GPIO_Pin = PWM2_Pin;
		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF;                      //复用功能             
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                     //推挽复用输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                       //上拉
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                             
		GPIO_Init(PWM_GPIOx,&GPIO_InitStructure);
	#endif
	
	#ifdef PWM3
		GPIO_PinAFConfig(PWM_GPIOx,PWM3_PinSource,PWM_AF_TIMx);
		/*配置电机输出的GPIO口*/
		GPIO_InitStructure.GPIO_Pin = PWM3_Pin;
		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF;                      //复用功能             
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                     //推挽复用输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                       //上拉
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                             
		GPIO_Init(PWM_GPIOx,&GPIO_InitStructure);
	#endif
	
	#ifdef PWM4
		GPIO_PinAFConfig(PWM_GPIOx,PWM4_PinSource,PWM_AF_TIMx);
		/*配置电机输出的GPIO口*/
		GPIO_InitStructure.GPIO_Pin = PWM4_Pin;
		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF;                      //复用功能             
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                     //推挽复用输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                       //上拉
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                             
		GPIO_Init(PWM_GPIOx,&GPIO_InitStructure);
	#endif
	
	/*定时器x初始化配置*/
	TIM_TimeBaseInitStructure.TIM_Period = arr;      //重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;   //分频值
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数
	TIM_TimeBaseInitStructure.TIM_ClockDivision  = TIM_CKD_DIV1;
	TIM_TimeBaseInit(PWM_TIMx,&TIM_TimeBaseInitStructure);
	
	/*PWM模式配置*/
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1   ;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High ;
	TIM_OCInitStructure.TIM_Pulse = 0                   ;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable  ;
	
	#ifdef PWM1
		TIM_OC1Init(PWM_TIMx,&TIM_OCInitStructure);
		TIM_OC1PreloadConfig(PWM_TIMx,TIM_OCPreload_Enable);
	#endif
	
	#ifdef PWM2
			TIM_OC2Init(PWM_TIMx,&TIM_OCInitStructure);
			TIM_OC2PreloadConfig(PWM_TIMx,TIM_OCPreload_Enable);
	#endif
	
	#ifdef PWM3
		TIM_OC3Init(PWM_TIMx,&TIM_OCInitStructure);
		TIM_OC3PreloadConfig(PWM_TIMx,TIM_OCPreload_Enable);
	#endif
	
	#ifdef PWM4
		TIM_OC4Init(PWM_TIMx,&TIM_OCInitStructure);
		TIM_OC4PreloadConfig(PWM_TIMx,TIM_OCPreload_Enable);
	#endif
	
	/*使能ARPE*/
	TIM_ARRPreloadConfig(PWM_TIMx,ENABLE);
	
	/*使能定时器2*/
	TIM_Cmd(PWM_TIMx,ENABLE);
}






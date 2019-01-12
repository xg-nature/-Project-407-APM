/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2014-04-27     Bernard      make code cleanup. 
 */

#include <board.h>
#include <rtthread.h>
#include "encode.h"
#include "led.h"
#include <usart.h>
#include "sample_time.h"
#include "delay.h"
#include "AD.h"
#include "bsp_dac8562_F407.h"
#include "pid.h"
#include "can.h"
#include "motion.h"


//extern rt_timer_t timer_pid;

/* 定时器控制块 */
//static rt_timer_t timer_test;
//static rt_tick_t timeout_value=4;

//rt_timer_t timer_4ms;
//static rt_tick_t timeout_value3=4;

//static rt_timer_t timer_1s;
//static rt_tick_t timeout_value2=1000;

//rt_timer_t timer_print;
//static rt_tick_t timeout_print=3000;



static rt_uint16_t count=0;
static rt_uint16_t count1=0;

//void timeout_callbak(void* parameter)
//{
//    
//	
//	count1 = TIM8->CNT;
//	
//	ResultValue =SeqIntPID1(4.05,0,32.5,4000,count1);	
//	//ResultValue =SeqIntPID(20,0,15,100,count);
//	
//	if(ResultValue>5400)
//		ResultValue=5400;
//	else if (ResultValue<-5400)
//		ResultValue=-5400;
//	//modbus_set_pwm((s16)ResultValue);	
//	//CAN_RoboModule_DRV_OpenLoop_Mode(0,1,1000);
//	
//	CAN_RoboModule_DRV_Current_Velocity_Mode(0,1,6000,(s16)ResultValue);
//	//CAN_RoboModule_DRV_Current_Velocity_Mode(0,1,6000,ResultValue);

//	
//	count = TIM4->CNT;
//	
////	count_1=count-count1;
////	count_1=(count_1+4096)%4096;

//	//ResultValue =SeqIntPID(4.6, 0,40,4000,count);   //km=0.45  T=360ms
//	ResultValue =SeqIntPID(80, 0,2400,count1*0.25,count);
////	if(mode == 0)
////		ResultValue =SeqIntPID(4.8, 0,60,0,count_1);
////    else
////		ResultValue =SeqIntPID(4.8, 0,60,clock_position,count_1);		
////	//ResultValue =SeqIntPID(0.5, 0,0,clock_position,count);
//	if(ResultValue>32767)
//		ResultValue=32767;
//	else if (ResultValue<-32768)
//		ResultValue=-32768;
//		
//	DAC8562_SetData(0, ResultValue+32768);
//	
//			
//}


//void timeout_callbak3(void* parameter)
//{
//    count1 = TIM8->CNT;
//	count = TIM4->CNT;
//	//count_1=count-count1;
//	//count_1=(count_1+8192)%8192;
//   // rt_kprintf("encoder1=%d\nencoder2=%d\n",count,count1);
//	//rt_kprintf("%d,%d",count,count1);
//	rt_kprintf("%d\n",count);
//	//rt_kprintf("%d\n",clock_position);
//}

//void timeout_callback_print(void* parameter)
//{
//    rt_timer_stop(timer_4ms);
//}



void app_init()
{	
	encode_Init();	
	//uart4_init(115200);
	CAN1_Configuration(); 
	 
	_delay_ms(500);                                      //发送复位指令后的延时必须要有，等待驱动器再次初始化完成
	bsp_InitDAC8562();
	 CAN_RoboModule_DRV_Reset(0,1);
	_delay_ms(500);
	bsp_InitDAC8562();
  
    CAN_RoboModule_DRV_Config(0,1,100,0);               //1号驱动器配置为100ms传回一次数据
    _delay_us(200);                                      //此处延时为了不让传回数据时候4个不一起传
	CAN_RoboModule_DRV_Mode_Choice(0,1,Current_Velocity_Mode);  //0组的所有驱动器 都进入开环模式
    _delay_ms(500);                                      //发送模式选择指令后，要等待驱动器进入模式就绪。所以延时也不可以去掉。
}


void rt_init_thread_entry(void* parameter)
{

	//rt_hw_led_init();	
					
	//Adc_Init();
	rt_enter_critical();
	
	encode_Init();
	
	
	
	CAN1_Configuration(); 
	 
	_delay_ms(500);                                      //发送复位指令后的延时必须要有，等待驱动器再次初始化完成
	bsp_InitDAC8562();
	 CAN_RoboModule_DRV_Reset(0,1);
	_delay_ms(500);
	bsp_InitDAC8562();
  
    CAN_RoboModule_DRV_Config(0,1,100,0);               //1号驱动器配置为100ms传回一次数据
    _delay_us(200);                                      //此处延时为了不让传回数据时候4个不一起传
	CAN_RoboModule_DRV_Mode_Choice(0,1,Current_Velocity_Mode);  //0组的所有驱动器 都进入开环模式
    _delay_ms(500);                                      //发送模式选择指令后，要等待驱动器进入模式就绪。所以延时也不可以去掉。
//	rt_timer_start(timer_pid);
		
	rt_exit_critical();
		
}






int rt_application_init()
{
//    rt_thread_t init_thread;
	
	app_init();
//	rt_kprintf("app_init\n");
	mycontrol_init2();
//	time_pid_init();
	
   
//    init_thread = rt_thread_create("init",
//        rt_init_thread_entry, RT_NULL,
//        2048, 8, 20);

//    if (init_thread != RT_NULL)
//        rt_thread_startup(init_thread);
	
//	timer_test = rt_timer_create("timer",  /* 定时器名字 timer1 */
//                                 timeout_callbak,  									/* 超时回调处理函数 */
//                                 RT_NULL,			 											/* 超时函数的入口参数 */
//                                 timeout_value, 											/* 定时长度，以OS Tick为单位,即timeout_value个OS Tick */
//                                 RT_TIMER_FLAG_PERIODIC); 						/* 周期性定时器 */
    /* 启动定时器 */
//    if (timer_test != RT_NULL)
//        rt_timer_start(timer_test);
	
//	timer_4ms = rt_timer_create("timer3",  /* 定时器名字 timer1 */
//                                 timeout_callbak3,  									/* 超时回调处理函数 */
//                                 RT_NULL,			 											/* 超时函数的入口参数 */
//                                 timeout_value3, 											/* 定时长度，以OS Tick为单位,即timeout_value个OS Tick */
//                                 RT_TIMER_FLAG_PERIODIC); 						/* 周期性定时器 */

    /* 启动定时器 */
//    if (timer_4ms != RT_NULL)
//        rt_timer_start(timer_4ms);

//timer_print = rt_timer_create("timer_print",  /* 定时器名字 timer1 */
//                                 timeout_callback_print,  									/* 超时回调处理函数 */
//                                 RT_NULL,			 											/* 超时函数的入口参数 */
//                                 timeout_print, 											/* 定时长度，以OS Tick为单位,即timeout_value个OS Tick */
//                                 RT_TIMER_FLAG_ONE_SHOT); 						/* 周期性定时器 */
//			          
			
    return 0;
}

/*@}*/

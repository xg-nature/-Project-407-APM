/**********************************************************************************************************
*
*	模块名称 : 功能演示
*	文件名称 : test.c
*	版    本 : V1.0
*	说    明 :
*	修改记录 :
*		版本号  日期        作者                        说明
*
*		v1.0    2013-4-20   jiezhi320(UP MCU 工作室)    演示信号量的实际使用
*
*	Copyright (C), 2012-2013,
*   淘宝店：   http://shop73275611.taobao.com
*   QQ交流群： 258043068
*
**********************************************************************************************************/
#include <rtthread.h>
#include <stm32f4xx.h>
#include "test.h"
//#include "key.h"
//#include "led.h"




void timeout_callbak(void* parameter);

/*  变量分配4字节对齐 */
ALIGN(RT_ALIGN_SIZE)

/*  静态线程的 线程堆栈*/
static rt_uint8_t key_stack[512];
/* 静态线程的 线程控制块 */

static struct rt_thread key_thread;

/* 定时器的控制块 */
static rt_timer_t timer_test;
static rt_tick_t timeout_value=10;

rt_err_t demo_start(void)
{
    rt_err_t result;

    rt_hw_led_init();
	  
    /* 创建key线程 ： 优先级 15 ，时间片 5个系统滴答 */
    result = rt_thread_init(&key_thread,
                            "key",
                            key_thread_entry, RT_NULL,
                            (rt_uint8_t*)&key_stack[0], sizeof(key_stack), 15, 5);

    if (result == RT_EOK)
    {
        rt_thread_startup(&key_thread);
    }
    /*创建动态定时器*/
    timer_test = rt_timer_create("timer1",  /* 定时器名字是 timer1 */
                                 timeout_callbak,  									/* 超时时回调的处理函数 */
                                 RT_NULL,			 											/* 超时函数的入口参数 */
                                 timeout_value, 											/* 定时长度，以OS Tick为单位，即timeout_value个OS Tick */
                                 RT_TIMER_FLAG_PERIODIC); 						/* 周期性定时器 */

    /* 启动定时器 */
    if (timer_test != RT_NULL)
        rt_timer_start(timer_test);

    rt_kprintf("timer started !\n");
    return 0;
}

rt_uint8_t flag;
rt_uint32_t count=0;


/* 超时时回调的处理函数 */
void timeout_callbak(void* parameter)
{
    flag^=1;
		count++;

    if (flag)
        rt_hw_led_off(0);
    else
        rt_hw_led_on(0);
    rt_kprintf("timer time out %d !\n",count);
		
}



/* 控制定时器 */
void timer_conrol(void)
{
    timeout_value+=10;
    /* 更改定时器超时时间*/
    rt_timer_control(timer_test, RT_TIMER_CTRL_SET_TIME, (void *)&timeout_value);
    rt_kprintf("timer timeout time set to %d !\n", timeout_value);                    
    if (timeout_value==500)
    {
        rt_timer_stop(timer_test); /* 停止定时器 */
        rt_kprintf("timer stoped !\n");
    }

    if (timeout_value>=510)
    {
        /* 再次启动定时器 */
        rt_timer_start(timer_test);
        timeout_value=10;
        rt_timer_control(timer_test, RT_TIMER_CTRL_SET_TIME, (void *)&timeout_value);
    }

}



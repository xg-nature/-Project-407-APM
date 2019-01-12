/********************
*包含三个线程
*数据采集线程
*电机控制线程
*数据输出线程
*不同线程之间采用消息队列进行通信
***********************/

#include <rtthread.h>
#include <string.h>
#include <finsh.h>
#include <math.h>
#include "encode.h"
#include "bsp_dac8562_F407.h"
#include "pid.h"
#include "motion.h" 
#include "sendata.h"//包含测试代码
#define MAX_ANGLE    30
#define MIN_ANGLE    -35
#define STEP_ANGLE   5
#define PRESICE      22.75
#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

/* 指向线程控制块的指针-电机控制线程*/
static rt_thread_t my_conyrol = RT_NULL;

static rt_uint16_t Desire_angle=0;
static rt_uint16_t count=0;


float kp=45,ki=0,kd=0;
u8 flag=1;


/* 消息队列控制块*/ 
static struct rt_messagequeue mq;
/* 消息队列内存池*/
static char msg_pool[512];

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

/* 指向线程控制块的指针-数据采集和数据输出*/
static rt_thread_t data_require = RT_NULL;
static rt_thread_t data_send = RT_NULL;


//采集数据线程入口
static void thread_data_require_entry(void* parameter)
{		
	struct rt_data data_receive={0,0,0};//定义结构体保存采集的数据
		int result;
	
    while (1)
    {
			//采集数据程序
			#ifndef TEST
				data_receive.encode2_data= TIM4->CNT;
			  data_receive.motor_data=TIM8->CNT;
			#endif
			
			//测试程序
			#if defined TEST
			  data_receive.motor_data=88;				
				data_receive.encode2_data= 55;
			#endif
			
			  data_receive.mytick= rt_tick_get();
			  //将采集的数据发送给消息队列
				result = rt_mq_send(&mq, &data_receive, sizeof(data_receive));
            if ( result == -RT_EFULL)
            {
                rt_kprintf("message queue full, delay 10tick\n");
                rt_thread_delay(10);
            }
			
        rt_thread_delay(10);
    }
}

//数据采集线程控制块 TCB
int data_require_init()
{
    data_require = rt_thread_create("t1",
        thread_data_require_entry, (void*)1, 
        THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (data_require  != RT_NULL)
        rt_thread_startup(data_require );
    else
        return -1;
    return 0;
}
//数据输出线程入口
static void thread_data_send_entry(void* parameter)
{		
	//数据帧的开始与结束
		u16 start=0xffff;
		u16 end=0xfffe;
	//发送数据的方式
		rt_device_t usart3_send;
		usart3_send = rt_device_find("uart3");
		struct rt_data count_data={0,0,0};
		if(usart3_send!=RT_NULL)
		{
				rt_device_open(usart3_send, RT_DEVICE_OFLAG_RDWR);//打开设备并初始化
				while(1)
				{		
					
//从消息队列获取数据采集的信息					
						if (rt_mq_recv(&mq, &count_data, sizeof(count_data), RT_WAITING_FOREVER)== RT_EOK)
						{		
								#if defined (TEST)
								rt_kprintf("电机:%d\t编码器2数据:%d\t时间戳:%d\n",count_data.motor_data,count_data.encode2_data,count_data.mytick);
								#endif	
							rt_device_write(usart3_send,0,&start,sizeof(start));
							rt_device_write(usart3_send,0,&count_data,sizeof(count_data));
							rt_device_write(usart3_send,0,&end,sizeof(end));
						}
						rt_thread_delay(5);
				}	
		}
			
}

//创建数据发送控制块
int data_send_init()
{
    data_send = rt_thread_create("t2",
        thread_data_send_entry, (void*)1, 
        THREAD_STACK_SIZE, THREAD_PRIORITY+2, THREAD_TIMESLICE);
    if (data_send  != RT_NULL)
        rt_thread_startup(data_send );
    else
        return -1;
    return 0;
}

//创建消息队列控制块
int messageq_simple_init()
{
    rt_mq_init(&mq, "mqt", 
        &msg_pool[0],
        sizeof(struct rt_data), 
        sizeof(msg_pool),
        RT_IPC_FLAG_FIFO);
	  return 0;
}


void setpid(int argc, char **argv)
{
	// float tkp,tki,tkd;
	if (argc == 4)
    {
        kp = strtol(argv[1], 0, 10);
				ki = strtol(argv[2], 0, 10);
				kd = strtol(argv[3], 0, 10);		
    }
}

MSH_CMD_EXPORT(setpid, setpid); 

//rt_timer_t timer_pid;
//static rt_tick_t timeout_pid=4;

//void pid_callbak(void* parameter)
//{	int ResultValue;
//	count = TIM4->CNT;
//	ResultValue =SeqIntPID(kp, ki,kd,Desire_angle,count);
//	if(ResultValue>1000)
//		ResultValue=1000;
//	else if (ResultValue<-1000)
//		ResultValue=-1000;	
//	DAC8562_SetData(0, ResultValue+32768);
//}
//void time_pid_init()
//{
//    timer_pid = rt_timer_create("timer_pid",  
//                                 pid_callbak,  									
//                                 RT_NULL,			 											
//                                 timeout_pid, 											
//                                 RT_TIMER_FLAG_PERIODIC); 						
//	
//	rt_kprintf("time_pid_init\r\n");

//    if(timer_pid != RT_NULL)
//		rt_timer_start(timer_pid);

//}

void setzero(int argc, char **argv)
{
	TIM8->CNT=0;
	TIM4->CNT=0;
	Desire_angle=0;
	rt_kprintf("%d\n",TIM4->CNT);
}
MSH_CMD_EXPORT(setzero, setzero); 
  
void setposition(int argc, char **argv)
{
	int angle;
	
	 if (argc < 2)
    {
        rt_kprintf("Usage: setposition\n");
        rt_kprintf("Like: setposition 20\n");
        return ;
    }
	
	angle = strtol(argv[1], 0, 10);
	if(angle>=0)
	Desire_angle=angle*PRESICE;	
	else
	Desire_angle=8192+angle*PRESICE;
//	rt_timer_start(timer_print);
//	rt_timer_start(timer_4ms);
	//rt_thread_delay(RT_TICK_PER_SECOND/2);
//	rt_kprintf("%d\n",count);
}

MSH_CMD_EXPORT(setposition, setposition); 

void headle()
{
	int i=10;
	while(i--)
	{
		rt_kprintf("%d,%d\n",TIM4->CNT,TIM8->CNT);
		rt_thread_delay(RT_TICK_PER_SECOND/200);	
	}
}

void auto_measure(int argc, char **argv)
{
	int angle=MIN_ANGLE;
	while(angle!=MAX_ANGLE)
	{
		if(angle>=0)
		Desire_angle=angle*PRESICE;	
		else
		Desire_angle=8192+angle*PRESICE;
		while(fabs(Desire_angle-count)>PRESICE);
		rt_thread_delay(RT_TICK_PER_SECOND);
		
		headle();	
		
		angle=angle+STEP_ANGLE;
	}
}

MSH_CMD_EXPORT(auto_measure, auto_measure); 



//电机控制线程入口
static void thread_entry(void* parameter)
{			
	  int ResultValue;
    while (1)
    {
				count = TIM4->CNT;
				ResultValue =SeqIntPID(kp, ki,kd,Desire_angle,count);
				if(ResultValue>1000)
					ResultValue=1000;
				else if (ResultValue<-1000)
					ResultValue=-1000;	
				DAC8562_SetData(0, ResultValue+32768);
        rt_thread_delay(4);
    }
}

//创建电机控制线程及初始化
int mycontrol_init2()
{
    my_conyrol = rt_thread_create("t1",
        thread_entry, (void*)1, 
        THREAD_STACK_SIZE, THREAD_PRIORITY+1, THREAD_TIMESLICE);
    if (my_conyrol != RT_NULL)
        rt_thread_startup(my_conyrol);
    else
        return -1;
		messageq_simple_init();
		data_send_init();
		data_require_init();
    return 0;
}




#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "delay.h"

u8 receCount;
u8 receBuf[32];
u8 sendCount;	 
u8 sendBuf[120];
u8 sendPosi;
u8 checkoutError;

u8 receCount2;
u8 receBuf2[32];
u8 sendCount2;	 
u8 sendBuf2[32];
u8 sendPosi2;
u8 checkoutError2;

void uart1_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	   //AFIO 功能复用
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;  //抢占3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		// 优先1

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
  
   //USART 初始化设置
   
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Odd;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
   

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启接受中断
//	USART_ClearFlag(USART1, USART_FLAG_TC);		  	//清零发送置位标准!!
	USART_ITConfig(USART1,USART_IT_TC,ENABLE);	   //开发送中断   
    USART_Cmd(USART1, ENABLE);                    //使能串口
}


void uart2_init(u32 bound)					//串口2配置
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
     //USART2_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		   //推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //上拉悬空
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart2 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;	//	抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//	响应优先级2

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
  
   //USART 初始化设置
   
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;	  //设置奇偶校验以后，数据位的第九位就是奇偶校验位！！
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Parity = USART_Parity_Odd; //奇校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);   

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启接受中断
//	USART_ClearFlag(USART2, USART_FLAG_TC);		  	//清零发送置位 ???????
	USART_ITConfig(USART2,USART_IT_TC,ENABLE);	   //开发送中断   
    USART_Cmd(USART2, ENABLE);                    //使能串口 

}

void beginSend(void)
{	
	sendEnable();	//设为发送//
	sendPosi = 0;
	if(sendCount > 1)
		sendCount--;
	USART_SendData(USART1,sendBuf[0]);
}


void beginSend2(void)
{	
	sendEnable2();	//设为发送//
	sendPosi2 = 0;
	if(sendCount2 > 1)
		sendCount2--;
	USART_SendData(USART2,sendBuf2[0]);
}


void USART1_IRQHandler(void)                	//串口1中断服务程序
{
//	if(USART1->SR&(1<<5))//接收到数据
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		if(USART_GetFlagStatus(USART1,USART_FLAG_PE)==RESET)
		{
//			receBuf[receCount] =USART1->DR;
			receBuf[receCount]= USART_ReceiveData(USART1);	 
	    	receCount++;          //接收地址偏移寄存器加1
			Timer3_enable();
		}
	} 
	else if(USART_GetITStatus(USART1,USART_IT_TC) == SET)
	{
		USART_ClearFlag(USART1,USART_FLAG_TC);		 //注意这个地方的写法，stm32串口发送中断的写法,要清零标志位
		if(sendPosi < sendCount) //character has been transmitted
	    {
	        sendPosi++;
	        USART_SendData(USART1,sendBuf[sendPosi]);
	    }
	    else
	    {
	        receEnable();    //发送完后将485置于接收状态
	        receCount = 0;   //清接收地址偏移寄存器//
	        checkoutError = 0;
	    } 
	}	
} 

void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) ==SET)		  //接收中断
	{
		if(USART_GetFlagStatus(USART2,USART_FLAG_PE)==RESET) //检查奇偶校验是否错误
		{
			receBuf2[receCount2]= USART_ReceiveData(USART2);
	    	receCount2++;          //接收地址偏移寄存器加1
			Timer2_enable();
		}
	}

	else if(USART_GetITStatus(USART2,USART_IT_TC) == SET)
	{
		USART_ClearFlag(USART2,USART_FLAG_TC);		 //注意这个地方的写法，stm32串口发送中断的写法,要清零标志位
		if(sendPosi2 < sendCount2)
	    {
	        sendPosi2++;
	        USART_SendData(USART2,sendBuf2[sendPosi2]);
	    }
	    else
	    {
	        receEnable2();    //发送完后将485置于接收状态
	        receCount2 = 0;   //清接收地址偏移寄存器//
	        checkoutError2 = 0;
	    } 
	}	
} 

#ifndef	__USART2_H
#define	__USART2_H

#include "stm32f4xx.h" 

#define MY_USART2_REC_LEN  			20  	//定义最大接收字节数 200
/*****************MY_USART1*****************************/
#define	MY_USART2							3
#define MY_USART2_EN_RX   												//是否开启接收中断 ，不开启可注释掉该行
#define MY_USART2_USARTx					USART3						//串口 号
#define	MY_USART2_AHBxPeriph_GPIOx			RCC_AHB1Periph_GPIOD		//串口 对应GPIO时钟
#define	MY_USART2_APBxPeriph_USARTx			RCC_APB1Periph_USART3		//串口 时钟
#define	MY_USART2_PinSource1				GPIO_PinSource8				//串口 复用引脚1
#define	MY_USART2_PinSource2				GPIO_PinSource9				//串口 复用引脚2
#define	MY_USART2_AF_USARTx					GPIO_AF_USART3				//串口 复用功能
#define	MY_USART2_GPIOx						GPIOD						//串口 GPIO
#define	MY_USART2_TX_Pin					GPIO_Pin_8					//串口 TX脚
#define	MY_USART2_RX_Pin					GPIO_Pin_9					//串口 RX脚

#define MY_USART2_IRQn						USART3_IRQn					//串口 中断向量号

/****************************************************/

void uart2_init(u32 bound);


#endif

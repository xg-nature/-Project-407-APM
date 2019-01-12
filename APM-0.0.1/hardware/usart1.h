#ifndef	__USART1_H
#define	__USART1_H

#include "stm32f4xx.h" 

#define MY_USART1_REC_LEN  			1024  	//定义最大接收字节数 200
/*****************MY_USART1*****************************/
#define	MY_USART1							2
#define MY_USART1_EN_RX   													//是否开启接收中断 ，不开启可注释掉该行
#define MY_USART1_USARTx					USART2						//串口 号
#define	MY_USART1_AHBxPeriph_GPIOx			RCC_AHB1Periph_GPIOD		//串口 对应GPIO时钟
#define	MY_USART1_APBxPeriph_USARTx			RCC_APB1Periph_USART2		//串口 时钟
#define	MY_USART1_PinSource1				GPIO_PinSource5				//串口 复用引脚1
#define	MY_USART1_PinSource2				GPIO_PinSource6				//串口 复用引脚2
#define	MY_USART1_AF_USARTx					GPIO_AF_USART2				//串口 复用功能
#define	MY_USART1_GPIOx						GPIOD						//串口 GPIO
#define	MY_USART1_TX_Pin					GPIO_Pin_5					//串口 TX脚
#define	MY_USART1_RX_Pin					GPIO_Pin_6					//串口 RX脚

#define MY_USART1_IRQn						USART2_IRQn					//串口 中断向量号

/****************************************************/

extern	u8 	MY_USART1_RX_BUF[MY_USART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.


void uart1_init(u32 bound);


#endif

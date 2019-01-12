/*
 * File      : usart.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */
#include "stm32f4xx.h"
#ifndef __USART_H__
#define __USART_H__

#include <rthw.h>
#include <rtthread.h>

#define UART_ENABLE_IRQ(n)            NVIC_EnableIRQ((n))
#define UART_DISABLE_IRQ(n)           NVIC_DisableIRQ((n))

int stm32_hw_usart_init(void);

//#define UART4_REC_LEN  			200  	//定义最大接收字节数 200
//#define EN_UART4_RX 			1		//使能（1）/禁止（0）串口1接收
//extern u8  UART4_RX_BUF[UART4_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
//extern u16 UART4_RX_STA;         		//接收状态标记	
////如果想串口中断接收，请不要注释以下宏定义
//void uart4_init(u32 bound);
#endif

#include "usart2.h"	


void uart2_init(u32 bound){
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(MY_USART2_AHBxPeriph_GPIOx,ENABLE); //使能GPIOD时钟
	RCC_APB1PeriphClockCmd(MY_USART2_APBxPeriph_USARTx,ENABLE);//使能USART2时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(MY_USART2_GPIOx,MY_USART2_PinSource1,MY_USART2_AF_USARTx); //GPIOD5复用为USART2
	GPIO_PinAFConfig(MY_USART2_GPIOx,MY_USART2_PinSource2,MY_USART2_AF_USARTx); //GPIOD6复用为USART2
	
	//USART2端口配置
	GPIO_InitStructure.GPIO_Pin = MY_USART2_TX_Pin | MY_USART2_RX_Pin; //GPIOD5与GPIOD6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(MY_USART2_GPIOx,&GPIO_InitStructure); //初始化PD5，PD6

   //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;						//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(MY_USART2_USARTx, &USART_InitStructure);				 //初始化串口
	
	USART_Cmd(MY_USART2_USARTx, ENABLE);  //使能串口 
	
#ifdef MY_USART2_EN_RX	
	USART_ITConfig(MY_USART2_USARTx, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = MY_USART2_IRQn;	//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							//根据指定的参数初始化VIC寄存器、

#endif
	
}

#ifdef	MY_USART2_EN_RX	
	//bit15，	接收完成标志
	//bit14，	接收到0x0d
	//bit13~0，	接收到的有效字节数目
	u16 MY_USART2_RX_STA=0;      			 //接收状态标记	
	u8 	MY_USART2_RX_BUF[MY_USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
#endif

#if MY_USART2==3
	void USART3_IRQHandler(void)                	//串口2中断服务程序
	{
		u8 Res;
		if(USART_GetITStatus(MY_USART2_USARTx, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
			Res =USART_ReceiveData(MY_USART2_USARTx);//(USART1->DR);	//读取接收到的数据
			
			if((MY_USART2_RX_STA&0x8000)==0)//接收未完成
			{
				if(MY_USART2_RX_STA&0x4000)//接收到了0x0d
				{
					if(Res!=0x0a)MY_USART2_RX_STA=0;//接收错误,重新开始
					else MY_USART2_RX_STA|=0x8000;	//接收完成了 
				}
				else //还没收到0X0D
				{	
					if(Res==0x0d)MY_USART2_RX_STA|=0x4000;
					else
					{
						MY_USART2_RX_BUF[MY_USART2_RX_STA&0X3FFF]=Res ;
						MY_USART2_RX_STA++;
						if((MY_USART2_RX_STA&0X3FFF)>(MY_USART2_REC_LEN-1))MY_USART2_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
		}
	}
#endif	






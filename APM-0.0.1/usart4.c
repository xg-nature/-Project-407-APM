#include "usart.h"
#include "sys.h"	
	
//u8 UART4_RX_BUF[UART4_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//u16 UART4_RX_STA=0;       //接收状态标记	

void uart4_init(u32 bound){
   
	//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能UART4时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11复用为UART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10复用为UART3
	
	//USART4端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOB10与GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PC11，PC10

   //USART4 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure); //初始化串口4
	
  USART_Cmd(USART3, ENABLE);  //使能串口4 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启接收中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口4中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

	
}


//void USART3_IRQHandler(void)                	//串口1中断服务程序
//{
//	u8 Res;
//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//	{
//		Res =USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据
//		
//		if((UART4_RX_STA&0x8000)==0)//接收未完成
//		{
//			if(UART4_RX_STA&0x4000)//接收到了0x0d
//			{
//				if(Res!=0x0a)UART4_RX_STA=0;//接收错误,重新开始
//				else {UART4_RX_STA|=0x8000;	//接收完成了
//							UART4_RX_STA=0;
//						//	USART_SendData(UART4,0x54);
//				}
//			}
//			else //还没收到0X0D
//			{	
//				if(Res==0x0d)UART4_RX_STA|=0x4000;//USART_RX_STA bit14表示是否接受到了0X0D
//				else
//				{
//					UART4_RX_BUF[UART4_RX_STA&0X3FFF]=Res ;
//					UART4_RX_STA++;
//					if(UART4_RX_STA>(UART4_REC_LEN-1)) UART4_RX_STA=0;//接收数据错误,重新开始接收	  
//				}		 
//			}
//		}   		 
//  } 


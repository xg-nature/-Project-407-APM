#include "datascope.h"
#include "DataScope_DP.h"
#include "usart1.h"
#include "control.h"

/******************************/
//需要用到的头文件
#include "encoder.h"
/******************************/

/*************上位机延时变量*****************/
u8 DataScope_Delay_Flag=0;
u8 DataScope_Flag_50ms=0;

/********************************************/

/*******************************/
//其他文件引用过来的变量
extern short Encoder1_Speed;


/*******************************/



/*发送数据到上位机*/
void DataScope(void)
{   
	u8 i;
	u8 DataScope_Send_Count=0; //串口需要发送的数据个数
	
	DataScope_Get_Channel_Data(Encoder1_Speed, 1);      
	DataScope_Get_Channel_Data(30, 2);                      
	//DataScope_Get_Channel_Data(0, 3);
	//DataScope_Get_Channel_Data(0, 4);
	//DataScope_Get_Channel_Data(0, 5);
	//DataScope_Get_Channel_Data(0, 6);
	
	
	DataScope_Send_Count = DataScope_Data_Generate(2);
	for( i = 0 ; i < DataScope_Send_Count; i++) 
	{
		while((MY_USART1_USARTx->SR&0X40)==0);  
		MY_USART1_USARTx->DR = DataScope_OutPut_Buffer[i]; 
	}
	DataScope_Delay_Flag=1;	          //===50ms中断精准延时标志位  	
	while(DataScope_Delay_Flag);        //===50ms中断精准延时  主要是波形显示上位机需要严格的50ms传输周期  

}

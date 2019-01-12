#ifndef __KEY_H
#define __KEY_H

#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 
//#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2
#define WK_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0


#define KEY0_PRES 	1
#define KEY1_PRES	2
//#define KEY2_PRES	3
#define WKUP_PRES   4




void key_thread_entry(void* parameter);
void rt_hw_key_init(void);



#endif /* __KEY_H */


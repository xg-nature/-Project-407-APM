#ifndef	__DATASCOPE_H
#define __DATASCOPE_H

#include "stm32f4xx.h" 

/***********相应外设开关***************/
#define	DATASCOPE					//上位机

/**************************************/

extern	u8 DataScope_Delay_Flag;
extern	u8 DataScope_Flag_50ms;

void DataScope(void);

#endif

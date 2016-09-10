#ifndef __CPU_INIT_H
#define	__CPU_INIT_H

#include "stm32f10x.h"

void RCC_Config(void); 
void EXTIX_Init(void);

#define Volume_Dn GPIO_Pin_0 //PA0
#define Volume_Up GPIO_Pin_1 //PA1

#endif // __CPU_INIT_H

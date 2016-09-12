#ifndef __CPU_INIT_H
#define	__CPU_INIT_H

#include "stm32f10x.h"

void RCC_Config(void); 
void EXTIX_Init(void);

#define GPIO_LED   GPIO_Pin_5 //PB5
    
#define Volume_Dn GPIO_Pin_0 //PA0
#define Volume_Up GPIO_Pin_1 //PA1

#define LCD0_BL_EN    GPIO_Pin_3 //背光 PA3
#define LCD0_BIAS_PEN GPIO_Pin_6 //使能 PA6
#define LCD0_BIAS_NEN GPIO_Pin_7 //使能 PA7

#define TC_RESET_L    GPIO_Pin_1  //PB1

#define MCU_OLED_1_RST GPIO_Pin_13 //PC13
#define MCU_OLED_0_RST GPIO_Pin_14 //PC14

#endif // __CPU_INIT_H

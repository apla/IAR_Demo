#ifndef __CPU_INIT_H
#define	__CPU_INIT_H
#include "stm32f10x.h"

void RCC_Config(void); 
void EXTIX_Init(void);


#define GPIO_LED   GPIO_Pin_5 //PB5
//系统音量上下键 
#define Volume_Up GPIO_Pin_1 //PA1
#define Volume_Dn GPIO_Pin_0 //PA0
//LCD背光PWM信号，LCD正负5V偏压信号
#define LCD0_BL_EN    GPIO_Pin_3 //背光 PA3
#define LCD0_BIAS_PEN GPIO_Pin_6 //使能 PA6
#define LCD0_BIAS_NEN GPIO_Pin_7 //使能 PA7
//TC358870XBG复位信号
#define TC_RESET_L    GPIO_Pin_1  //PB1
//OLED0,OLED1 复位信号
#define MCU_OLED_0_RST GPIO_Pin_14 //PC14
#define MCU_OLED_1_RST GPIO_Pin_13 //PC13


#ifdef debug1
    #define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
    #define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
    #define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

    #define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
    #define TRCENA          0x01000000

struct __FILE { int handle; /* Add whatever you need here */ };
    FILE __stdout;
    FILE __stdin;

    int fputc(int ch, FILE *f) {
      if (DEMCR & TRCENA) {
        while (ITM_Port32(0) == 0);
        ITM_Port8(0) = ch;
      }
      return(ch);
 }
#endif
    
 
#endif // __CPU_INIT_H

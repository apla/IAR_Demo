#include "stm32f10x.h"
#include "delay.h"
void LED_Init(void)
{
	 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //¨º1?¨¹PB,PE???¨²¨º¡À?¨®
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 ???¨²????
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //¨ª?¨ª¨¬¨º?3?
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO?¨²?¨´?¨¨?a50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //?¨´?Y¨¦¨¨?¡§2?¨ºy3?¨º??¡¥GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 ¨º?3???

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 ???¨²????, ¨ª?¨ª¨¬¨º?3?
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //¨ª?¨ª¨¬¨º?3? ¡ê?IO?¨²?¨´?¨¨?a50MHz
 GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 ¨º?3??? 
}
void delay(uint32_t t)
{
  while(--t);
}
int main(void)
{
        delay_init();
	LED_Init();		        //3?¨º??¡¥LED???¨²
	while(1)
	{
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);  //LED0??¨®|¨°y??GPIOB.5¨¤-¦Ì¨ª¡ê?¨¢¨¢  ¦Ì¨¨¨ª?LED0=0;
			GPIO_SetBits(GPIOE,GPIO_Pin_5);   //LED1??¨®|¨°y??GPIOE.5¨¤-??¡ê??e ¦Ì¨¨¨ª?LED1=1;
			delay_ms(200);  		   //?¨®¨º¡À300ms
                        delay_us(1000000);
			GPIO_SetBits(GPIOB,GPIO_Pin_5);	   //LED0??¨®|¨°y??GPIOB.5¨¤-??¡ê??e  ¦Ì¨¨¨ª?LED0=1;
			GPIO_ResetBits(GPIOE,GPIO_Pin_5); //LED1??¨®|¨°y??GPIOE.5¨¤-¦Ì¨ª¡ê?¨¢¨¢ ¦Ì¨¨¨ª?LED1=0;
			delay_ms(200);                   //?¨®¨º¡À300ms
                        delay_us(1000000);
	}
	
}
#undef TTT

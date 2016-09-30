
#include "Constants.h"

void RCC_Config(void)
{
    ErrorStatus HSEStartUpStatus;
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();
    
    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);
    
    do
        {
    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
        }while(HSEStartUpStatus!= SUCCESS);

    if(HSEStartUpStatus == SUCCESS)
    {
        /* HCLK = SYSCLK */
        RCC_HCLKConfig(RCC_SYSCLK_Div1); 
     //   RCC_HCLKConfig(RCC_SYSCLK_Div2); //16MHZ /2 = 8MHZ
        
        /* PCLK2 = HCLK */
        RCC_PCLK2Config(RCC_HCLK_Div1); 
        
        /* PCLK1 = HCLK/2 */
        RCC_PCLK1Config(RCC_HCLK_Div2);
        
        /* Flash 2 wait state */
        FLASH_SetLatency(FLASH_Latency_2);
        
        /* Enable Prefetch Buffer */
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        
#ifdef STICK
        /* PLLCLK = 8MHz * 6 = 48 MHz */
        RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_6);
        //  RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_9); //  72/1.5 = 48
#else        
        /* PLLCLK = 8MHz * 9 = 72 MHz */
        //Ido - set to Div2 ????
        RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_9);
#endif

        /* Enable PLL */ 
        RCC_PLLCmd(ENABLE);

        /* Wait till PLL is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }
        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x08)
        {
        }

    }
    else
    {
        /* If none of the define above is enabled, the HSI is used as System clock
        source (default after reset) */ 
        /* Disable HSE */
        RCC_HSEConfig(RCC_HSE_OFF);

        RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_9);

        RCC_PLLCmd(ENABLE);

        /* Wait till PLL is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x08)
        {
        }
    }
}

//音量按键IO初始化
static void KEY_Init(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;        
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = Volume_Dn | Volume_Up;//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入 GPIO_Mode_IPD
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//外部中断初始化        
void EXTIX_Init(void)
{
   	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
        KEY_Init();//按键初始化
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//复用功能使能
 	
        //PA0_Volume_Dn
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); 
  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_Init(&EXTI_InitStructure);
        //PA1_Volume_Up pcb_up.
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1); 
  	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_Init(&EXTI_InitStructure);	           

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;		
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//子优先级3					
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
  	NVIC_Init(&NVIC_InitStructure); 

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  	NVIC_Init(&NVIC_InitStructure);    	 
}

/*************************END OF FILE*************************************/

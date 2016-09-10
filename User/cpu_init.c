
#include "cpu_init.h"

void GPIO_Config(void)
{ 
      GPIO_InitTypeDef  GPIO_InitStructure; 	
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
      GPIO_Init(GPIOB, &GPIO_InitStructure);					
      GPIO_SetBits(GPIOB,GPIO_Pin_5);//PB5_LED

      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
      GPIO_Init(GPIOA, &GPIO_InitStructure);//PA3_dutycircle 
      GPIO_SetBits(GPIOB,GPIO_Pin_5);
}

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

/*************************END OF FILE*************************************/

/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usb_istr.h"
#include "delay.h"
#include "Constants.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

void USB_LP_CAN1_RX0_IRQHandler(void)//start.s
{
    USB_Istr();
}
static const u8 STEP_SIZE = 1;
static u8 start_data = 1;
void EXTI0_IRQHandler(void) //Volume_Dn;PA0
{
       u8 i,cnt_pulse=0;
       //最开始为1 datesheet:p8  (1-32)      
       start_data += STEP_SIZE;
       if(start_data > 32){//每次变化1个单位
          start_data = 1;
          cnt_pulse = 32 + 32 - 1;
       }else{
          cnt_pulse = 1+STEP_SIZE + 32 -1;//after + 32 - before
       }
  //   GPIO_ResetBits(GPIOA,LCD0_BL_EN);//背光 LCD0_BL_EN
       delay_ms(3);  
       for(i=0;i< cnt_pulse;i++){  //1-32(less-)  
           GPIO_SetBits(GPIOA,LCD0_BL_EN);
           delay_us(1);
           GPIO_ResetBits(GPIOA,LCD0_BL_EN);
           delay_us(1);
       }
     GPIO_SetBits(GPIOA,LCD0_BL_EN);
     GPIO_WriteBit(GPIOB, GPIO_Pin_5,(BitAction)(1-(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5))));
     EXTI_ClearITPendingBit(EXTI_Line0); 
}
//Option 1: send in 24 pulses (19(start)+32-27(end) = 24);
//Option 2: pull low for 3ms and then send in 19 pulses.
void EXTI1_IRQHandler(void)//Volume_Up;PA1
{
       u8 i,cnt_pulse=0;
       //最开始为1 datesheet:p8  (1-32)     
       start_data -= STEP_SIZE;
       if(start_data < 1){//每次变化1个单位
          start_data = 32;
          cnt_pulse = 1+32-32;
       }else{
          cnt_pulse = 1+ 32 -(STEP_SIZE+1);//降低亮度
       }
  //   GPIO_ResetBits(GPIOA,LCD0_BL_EN);//背光 LCD0_BL_EN
       delay_ms(3);  
       for(i=0;i< cnt_pulse;i++){  //1-32(less-)  
           GPIO_SetBits(GPIOA,LCD0_BL_EN);
           delay_us(1);
           GPIO_ResetBits(GPIOA,LCD0_BL_EN);
           delay_us(1);
       }
     GPIO_SetBits(GPIOA,LCD0_BL_EN);
     GPIO_WriteBit(GPIOB, GPIO_Pin_5,(BitAction)(1-(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5))));
    EXTI_ClearITPendingBit(EXTI_Line1); 
}
void TIM2_IRQHandler(void)  // 1s enter
{
#if 0
     static BOOL flag = FALSE;
     if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
     {
       flag = ~flag;

     if(flag)
         GPIOB->BSRR = GPIO_Pin_5;
       else 
         GPIOB->BRR = GPIO_Pin_5;   
        if (TimeCount<0xff) TimeCount++;
      //   USB_SendString("Hi,PC! I'm STM32-ARM");
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 );	
     }  
#endif
}
/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

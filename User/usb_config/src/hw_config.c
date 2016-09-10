/**
  ******************************************************************************
  * @file    hw_config.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Hardware Configuration & Setup
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

ErrorStatus HSEStartUpStatus;
uint32_t ADC_ConvertedValueX = 0;
uint32_t ADC_ConvertedValueX_1 = 0;
__IO uint16_t  ADC1ConvertedValue = 0, ADC1ConvertedVoltage = 0, calibration_value = 0;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power.
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{  

   NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   //NVIC_VectTab_RAM

  GPIO_Configuration();
  
#if defined(USB_USE_EXTERNAL_PULLUP)
  /* Enable the USB disconnect GPIO clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIO_DISCONNECT, ENABLE);
  
  /* USB_DISCONNECT used as USB pull-up */
  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);  
#endif /* USB_USE_EXTERNAL_PULLUP */   
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz).
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
  // Select USBCLK source 
#ifdef stm32f103_eval
      RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);  // 72/1.5 = 48
#else
    //  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_Div1);
#endif
     RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_Div1);
     /* Enable USB clock */
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;
  
  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else 
  {
    bDeviceState = ATTACHED;
  }  
}

void USB_Interrupts_Config(void)
{
     NVIC_InitTypeDef NVIC_InitStructure; 
     NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;//USB_LP_IRQn
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
     // enable timer2
     NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
}

void Timer2Configuration(void)
{
     TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
     TIM_TimeBaseStructure.TIM_Period = 10000-1;  //  1s:20000  1ms*1000=1s
     TIM_TimeBaseStructure.TIM_Prescaler = 1600; //36MHZ/3600 =10Khz      
     TIM_TimeBaseStructure.TIM_ClockDivision = 0;   //36MHZ 
     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //Mode;
     TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
     TIM_Cmd(TIM2, ENABLE);
     TIM_ITConfig(TIM2,TIM_IT_CC1,ENABLE);
     TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 );	
}

void USB_Cable_Config (FunctionalState NewState)
{ 
        if (NewState != DISABLE)
	{
		GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);		//Á¬½ÓUSB
	}
	else
	{
		GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);	   //¶Ï¿ªUSB
	}
}

void GPIO_Configuration(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;       
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 ;	//USBDM and USBDP		 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits( GPIOA, GPIO_Pin_11 );
    GPIO_ResetBits( GPIOA, GPIO_Pin_12 );
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 ;	//USBDM and USBDP		 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //LCD_init_PB5
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;			 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 // GPIOB->BSRR = GPIO_Pin_5;
  GPIO_SetBits(GPIOB,GPIO_Pin_5);
  //???
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT
//                          , ENABLE);  
//  
//  /* USB_DISCONNECT used as USB pull-up */
//  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
//  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);
}
void DisableClock(void)
{
  //   RCC_APB1PeriphClockCmd(RCC_APB1Periph_ALL,DISABLE);
  //   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALL,DISABLE);
}

void Get_SerialNum(void)
{    
     u32 Device_Serial0, Device_Serial1;
     Device_Serial0 = *(u32*)(0x1FFFF7E8);
     Device_Serial1 = *(u32*)(0x1FFFF7EC);
     //Device_Serial2 = *(u32*)(0x1FFFF7F0);
     if (Device_Serial0 != 0)
     {
         CustomHID_StringSerial[2] = (uint8_t)(Device_Serial0 & 0x000000FF);  
         CustomHID_StringSerial[4] = (uint8_t)((Device_Serial0 & 0x0000FF00) >> 8);
         CustomHID_StringSerial[6] = (uint8_t)((Device_Serial0 & 0x00FF0000) >> 16);
         CustomHID_StringSerial[8] = (uint8_t)((Device_Serial0 & 0xFF000000) >> 24);  
        
         CustomHID_StringSerial[10] = (uint8_t)(Device_Serial1 & 0x000000FF);  
         CustomHID_StringSerial[12] = (uint8_t)((Device_Serial1 & 0x0000FF00) >> 8);
         CustomHID_StringSerial[14] = (uint8_t)((Device_Serial1 & 0x00FF0000) >> 16);
         CustomHID_StringSerial[16] = (uint8_t)((Device_Serial1 & 0xFF000000) >> 24); 
      }
}
#if 0
void USB_Connect (BOOL con) 
{
     GPIO_InitTypeDef GPIO_InitStructure;
     if (con)
     {
        GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
        GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);
     }
     else
     {
        GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
        GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure); 
     }
/*
     *CNTR = CNTR_FRES;                         // Force USB Reset  
     *ISTR = 0;                                 // Clear Interrupt Status  
     if (con) {
        *CNTR = CNTR_RESETM;                     // USB Reset Interrupt Mask  
     } else {
        *CNTR = CNTR_FRES | CNTR_PDWN;           // Switch Off USB Device  
     }
  */
}
#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/**
  ******************************************************************************
  * @file    usb_endp.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Endpoint routines
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
#include "usb_istr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Receive_Buffer[2];
#define nReportCnt 64
u8 Rx_Buffer[nReportCnt]="123456789a123456789a123456789a123456789a123456789a123456789a1234";
u8 Tx_Buffer[nReportCnt]="123456789a123456789a123456789a123456789a123456789a123456789a1234";
u8 USB_ReceiveFlg = FALSE;

extern __IO uint8_t PrevXferComplete;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : EP1_OUT_Callback.
* Description    : EP1 OUT Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
// 数据接收: USB_LP_CAN1_RX0_IRQHandler--->USB_Istr---->CTR_LP--->EPx_OUT_Callback
// 数据发送: UserToPMABufferCopy--->SetEPTxCount--->SetEPTxValid 
void EP1_OUT_Callback(void)
{
    volatile static bool flag = TRUE;
    flag = !flag;
    if(flag)
       GPIOB->BSRR = GPIO_Pin_5;
     else 
       GPIOB->BRR = GPIO_Pin_5;
     USB_ReceiveFlg = TRUE;//设置接收到数据标志位
     //将PMA双缓冲区中数据取到用户缓冲区
 //    PMAToUserBufferCopy(Receive_Buffer, ENDP1_RXADDR,nReportCnt);
//     MsgCmd = Receive_Buffer[21];
     //设置端点的接收状态为有效，因为端点接收到数据后会端点状态自动设置成停止状态
//     SetEPRxStatus(ENDP1, EP_RX_VALID);
 
}

/*******************************************************************************
* Function Name  : EP1_IN_Callback.
* Description    : EP1 IN Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_IN_Callback(void)
{
  //PrevXferComplete = 1;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


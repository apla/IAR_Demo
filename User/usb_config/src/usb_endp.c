/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MCD Application Team
* Version            : V2.2.1
* Date               : 09/22/2008
* Description        : Endpoint routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
#include "stm32f10x.h"
#include "usb_lib.h"
#include "usb_istr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 Rx_Buffer[nReportCnt]="123456789a123456789a123456789a123456789a123456789a123456789a1234";
u8 Tx_Buffer[nReportCnt]="123456789a123456789a123456789a123456789a123456789a123456789a1234";
u8 USB_ReceiveFlg = FALSE;
extern vu8 MsgCmd;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : EP1_OUT_Callback.
* Description    : EP1 OUT Callback Routine.�˵�1����ص�����
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
// ���ݽ���: USB_LP_CAN1_RX0_IRQHandler--->USB_Istr---->CTR_LP--->EPx_OUT_Callback
// ���ݷ���: UserToPMABufferCopy--->SetEPTxCount--->SetEPTxValid 
void EP1_OUT_Callback(void)
{     
//    volatile static bool flag = TRUE;
//    flag = !flag;
//    if(flag)
//       GPIOB->BSRR = GPIO_Pin_5;
//     else 
//       GPIOB->BRR = GPIO_Pin_5;
     USB_ReceiveFlg = TRUE;//���ý��յ����ݱ�־λ     
     GPIO_WriteBit(GPIOB, GPIO_Pin_5 ,(BitAction)(1-(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5))));
     
     
     //��PMA˫������������ȡ���û�������
 //    PMAToUserBufferCopy(Receive_Buffer, ENDP1_RXADDR,nReportCnt);
//     MsgCmd = Receive_Buffer[21];
     //���ö˵�Ľ���״̬Ϊ��Ч����Ϊ�˵���յ����ݺ��˵�״̬�Զ����ó�ֹͣ״̬
      SetEPRxStatus(ENDP1, EP_RX_VALID);
}

void EP2_IN_Callback(void)
{
//     u8 ii;
//     for (ii=0;ii<nReportCnt;ii++) Transi_Buffer[ii] = 0x00;
//     //for LED test
//     if (GPIOA->ODR & 0x0c )  GPIOA->ODR &= (~0x0c);
//     else GPIOA->ODR |= 0x0c;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/


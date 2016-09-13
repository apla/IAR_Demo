#include "stm32f10x.h"
#include "delay.h"
#include "public.h"
#include "usb_lib.h"
#include "i2c1_sensor.h"
#include "screen_iic.h" 

extern u8    USB_ReceiveFlg;
extern u8    Rx_Buffer[nReportCnt];
extern u8    Tx_Buffer[nReportCnt];
extern void USB_SendString(u8 *str);

int main(void)
{       
  //INTX_DISABLE();
  Set_System(); 
  EXTIX_Init();
  Screen_Init();
  // INTX_ENABLE();
  while(1){
      
  }
  
#ifdef USB_HID
  USB_Interrupts_Config();
  Set_USBClock();
  USB_Init();
  USB_ReceiveFlg = FALSE; 
  while(1){
        if(USB_ReceiveFlg == TRUE){
           USB_ReceiveFlg = FALSE;
           USB_SendString(Rx_Buffer);           
         }    
  }	
#endif
}

//���ݷ���: UserToPMABufferCopy--->SetEPTxCount--->SetEPTxValid 
void USB_SendString(u8 *str)
{
     u8 i=0;
     while(*str){
         Tx_Buffer[i++]=*(str++);
         if (i == nReportCnt) break;
     }
     //�������ݵ�PMA��
     UserToPMABufferCopy(Tx_Buffer, ENDP2_TXADDR, nReportCnt);
     //SetEPTxCount(ENDP2, nReportCnt); //�Ӷ˵�2����(nReportCnt)�ֽ�����
     SetEPTxValid(ENDP2);//ʹ�ܶ˵�2�ķ���״̬
     //GetEPTxStatus(ENDP1);����Ƿ��ͽ��� (EP_TX_VALID,EP_TX_NAK)
}


/******************* (C) COPYRIGHT 2011 Idealens *****END OF FILE****/
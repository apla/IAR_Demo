#include "stm32f10x.h"
#include "delay.h"
#include "Constants.h"
#include "usb_lib.h"
#include "i2c1_sensor.h"
#include "screen_iic.h" 
#include "mpu6500.h"
extern u8    USB_ReceiveFlg;
extern u8    Rx_Buffer[nReportCnt];
extern u8    Tx_Buffer[nReportCnt];
extern void USB_SendString(u8 *str);
extern  u8 InReport[];
int   temp1[3], temp2[3];

int main(void)
{       
  u8 temp=0;
  //INTX_DISABLE();
  Set_System(); 
  EXTIX_Init();  
  // INTX_ENABLE();
  //USB HID config
  USB_Interrupts_Config();
  Set_USBClock();
  USB_Init();
  USB_ReceiveFlg = FALSE; 
  //Shape_4k config
  Screen_Init();
  MPU6500_Init();
  while(1){
      InReport[1] = ++temp;
      MPU_ReadData( temp1 , temp2 );
    //  USB_SIL_Write(0x81, InReport,62);
      USB_SendString(InReport);
      delay_ms(300);
      GPIO_SetBits(GPIOB,GPIO_LED); 
      delay_ms(300);
      GPIO_ResetBits(GPIOB,GPIO_LED); 
  }
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
#include "stm32f10x.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "delay.h"
#include "cpu_init.h"
#include "i2c1_sensor.h"
extern u8 USB_ReceiveFlg;
 #define nReportCnt 64
extern u8 Rx_Buffer[nReportCnt];
extern u8 Tx_Buffer[nReportCnt];
     void USB_SendString(u8 *str); 
int main(void)
{  	
  u8 buf[255] = {0};
   NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
  RCC_Config();
  delay_init();
  //GPIO_Config();
  
  Set_System();
  USB_Interrupts_Config();
  Set_USBClock();
  USB_Init();
  USB_ReceiveFlg = FALSE;
  //I2C_Screen_Init();
  
  
  //I2C_ByteWrite(0x02,buf);
 // I2C_ByteWrite(0x01, 0x10);//LCM_AVEE: -5.6v 
 // I2C_ByteWrite(0x00, 0x12);//LCM_AVDD: 5.8v   
  
  //I2C_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead); 
  while(1){
//        if(USB_ReceiveFlg == TRUE){
//           USB_SendString("1TRUE==USB_ReceiveFlag");
//           delay_ms(500);          
//         }else{
//           USB_SendString("0FALSE==USB_ReceiveFlag");
//           delay_ms(500);
//         }
//      GPIO_ResetBits(GPIOB,GPIO_Pin_5);  
//      delay_ms(500);  		   
//      delay_us(1000000);
//      GPIO_SetBits(GPIOB,GPIO_Pin_5);
//      delay_ms(500);                 
//      delay_us(1000000);        
  }	
}

//���ݷ���: UserToPMABufferCopy--->SetEPTxCount--->SetEPTxValid 
void USB_SendString(u8 *str)
{
     u8 ii=0;
     while(*str)
     {
         Tx_Buffer[ii++]=*(str++);
         if (ii == nReportCnt) break;
     }
     //�������ݵ�PMA��
     UserToPMABufferCopy(Tx_Buffer, ENDP2_TXADDR, nReportCnt);
     //SetEPTxCount(ENDP2, nReportCnt); //�Ӷ˵�2����(nReportCnt)�ֽ�����
     SetEPTxValid(ENDP2);//ʹ�ܶ˵�2�ķ���״̬
     //GetEPTxStatus(ENDP1);����Ƿ��ͽ��� (EP_TX_VALID,EP_TX_NAK)
}

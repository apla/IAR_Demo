#include "stm32f10x.h"
#include "cpu_init.h"
#include "usb_lib.h"
#include "i2c1_sensor.h"

extern u8   USB_ReceiveFlg;
extern u8   Rx_Buffer[nReportCnt];
extern u8   Tx_Buffer[nReportCnt];
extern void USB_SendString(u8 *str);

int main(void)
{       
  Set_System(); 
  EXTIX_Init();
  while(1){
    
  }
  
#ifdef USB_HID
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
        if(USB_ReceiveFlg == TRUE){
           USB_ReceiveFlg = FALSE;
           USB_SendString(Rx_Buffer);           
         }
//      GPIO_ResetBits(GPIOB,GPIO_Pin_5);  
//      delay_ms(500);  		   
//      delay_us(1000000);
//      GPIO_SetBits(GPIOB,GPIO_Pin_5);
//      delay_ms(500);                 
//      delay_us(1000000);        
  }	
#endif
}

//数据发送: UserToPMABufferCopy--->SetEPTxCount--->SetEPTxValid 
void USB_SendString(u8 *str)
{
     u8 i=0;
     while(*str){
         Tx_Buffer[i++]=*(str++);
         if (i == nReportCnt) break;
     }
     //拷贝数据到PMA中
     UserToPMABufferCopy(Tx_Buffer, ENDP2_TXADDR, nReportCnt);
     //SetEPTxCount(ENDP2, nReportCnt); //从端点2发送(nReportCnt)字节数据
     SetEPTxValid(ENDP2);//使能端点2的发送状态
     //GetEPTxStatus(ENDP1);检测是否发送结束 (EP_TX_VALID,EP_TX_NAK)
}

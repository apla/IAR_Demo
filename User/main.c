#include "stm32f10x.h"
#include "delay.h"
#include "Constants.h"
#include "usb_lib.h"
#include "i2c1_sensor.h"
#include "screen_iic.h" 
#include "mpu6500.h"
#include "main.h"

int main(void)
{       
      // u8 temp=0;
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
      //MPU6500_Init();
      while(1){
          //tc358870_hotplug();
          /*
          InReport[1] = ++temp;
          MPU_ReadData( temp1 , temp2 );
        //  USB_SIL_Write(0x81, InReport,62);
          USB_SendString(InReport);
          delay_ms(300);
          GPIO_SetBits(GPIOB,GPIO_LED); 
          delay_ms(300);
          GPIO_ResetBits(GPIOB,GPIO_LED); */
      }
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
     UserToPMABufferCopy(Tx_Buffer, ENDP2_TXADDR, 62);//nReportCnt
     //SetEPTxCount(ENDP2, nReportCnt); //从端点2发送(nReportCnt)字节数据
     SetEPTxValid(ENDP2);//使能端点2的发送状态
     //GetEPTxStatus(ENDP1);检测是否发送结束 (EP_TX_VALID,EP_TX_NAK)
}

//HDMI 热插拔功能
void tc358870_hotplug(void)	
{
        u8 cbit = 0;
        //Wait until HDMI sync is established
        cbit = SM_ReadData(TCXXX_ADDRESS, 0x8520, 1, NULL); // SYS_STATUS Sequence: Check bit7 of 8x8520
        if((cbit>>7)&0x1)
        {            
            if(HDMI_PLUG == 0)
            {
              //SM_Init_TC358870XBG(); 
              //  sharp_lin_10801920();
                Screen_Config_1920_1080();
                sharp_lin_1440_2560();   
            }
            HDMI_PLUG = 1;
        }
        else
        {
            HDMI_PLUG = 0;
            //resset LCD
            PANEL_GPIO_RESET_PIN(TRUE);
        }                
        SmDelayms(100);

}

/******************* (C) COPYRIGHT 2011 Idealens *****END OF FILE****/
#include "stm32f10x.h"
#include "delay.h"
#include "cpu_init.h"
#include "i2c1_sensor.h"


      
int main(void)
{  	
  u8 buf[255] = {0};
  RCC_Config();
  delay_init();
  GPIO_Config();
  
  I2C_Screen_Init();
  
  
  //I2C_ByteWrite(0x02,buf);
 // I2C_ByteWrite(0x01, 0x10);//LCM_AVEE: -5.6v 
 // I2C_ByteWrite(0x00, 0x12);//LCM_AVDD: 5.8v   
  
  //I2C_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead); 
  while(1){
      GPIO_ResetBits(GPIOB,GPIO_Pin_5);  
      delay_ms(500);  		   
      delay_us(1000000);
      GPIO_SetBits(GPIOB,GPIO_Pin_5);
      delay_ms(500);                 
      delay_us(1000000);        
  }	
}


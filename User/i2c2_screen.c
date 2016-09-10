/********************************************************************************
 * 文件名  ：I2C2_screen.c
 * 描述    ：i2c 屏幕配置应用函数库  
 * 硬件连接：-----------------
 *          |                 |
 *          |  PB6-I2C1_SCL	  |
 *          |  PB7-I2C1_SDA   |
 *          |                 |
 *           -----------------
 * 库版本  ：ST3.5.0
**********************************************************************************/
#include "I2C2_screen.h"

#define I2C2_Speed              400000
#define I2C2_OWN_ADDRESS7      0x0A
#define I2C2_PageSize           8	

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  
//Mini STM32?a·￠°?ê1ó?μ?ê?24c02￡??ùò??¨ò?EE_TYPE?aAT24C02
#define EE_TYPE AT24C02
//初始化IIC接口
void Screen_Init(void)
{
	IIC_Init();
}
//在Screen指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
//宏定义有个选择芯片类型的，如果这个定义大于AT24C16的话就发送两个8bit地址，否则就发送一个8bit地址
u8 Screen_ReadByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
        IIC_Start();  
        
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址
		IIC_Wait_Ack();		 
	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 
        

	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //进入接收模式			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}
//在Screen指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void Screen_WriteByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
        IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址
 	}else
	{
		IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 
	}	 
	IIC_Wait_Ack();	   
        IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
        IIC_Stop();//产生一个停止条件 
	delay_ms(10);	 
}
//在Screen里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void Screen_WriteBytes(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		Screen_WriteByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//在Screen里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
u32 Screen_ReadBytes(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=Screen_ReadByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
//检查Screen是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
u8 Screen_Check(void)
{
	u8 temp;
	temp=Screen_ReadByte(255);//避免每次开机都写Screen			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		Screen_WriteByte(255,0X55);
	    temp=Screen_ReadByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//在Screen里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void Screen_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=Screen_ReadByte(ReadAddr++);	
		NumToRead--;
	}
}  
//在Screen里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void Screen_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		Screen_WriteByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}


/*************************END OF FILE*************************************/

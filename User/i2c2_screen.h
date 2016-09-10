#ifndef __I2C2_H
#define	__I2C2_H

#include "my_i2c.h"
#include "delay.h"

/* Slave Addresses defines */
#define SLAVE_ADDRESS (0x3e<<1) //0x7c 
#define TCXXX_ADDRESS 0x1e

//void I2C2_Screen_Init(void);
//void I2C2_BufferWrite(u8 WriteAddr,u8* pBuffer, u16 NumByteToWrite);
//void I2C2_ByteWrite(u8 WriteAddr,u8* pBuffer);
//void I2C2_PageWrite(u8 WriteAddr,u8* pBuffer,  u8 NumByteToWrite);
//void I2C2_BufferRead(u8 ReadAddr,u8* pBuffer,  u16 NumByteToRead);
//void I2C2_WaitEepromStandbyState(void);

u8   Screen_ReadByte(u16 ReadAddr);				//指定地址读取一个字节
void Screen_WriteByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
void Screen_WriteBytes(u16 WriteAddr,u32 DataToWrite,u8 Len);   //指定地址开始写入指定长度的数据
u32  Screen_ReadBytes(u16 ReadAddr,u8 Len);			//指定地址开始读取指定长度数据
void Screen_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void Screen_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据

u8   Screen_Check(void); //检查器件
void Screen_Init(void);  //初始化IIC

#endif // __I2C2_H

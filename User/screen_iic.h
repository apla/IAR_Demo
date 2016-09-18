#ifndef __SCREEN_IIC_H
#define __SCREEN_IIC_H

#include "myiic2.h"
					  
u8   Screen_ReadByte(u16 ReadAddr);				//指定地址读取一个字节
void Screen_WriteByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
void Screen_WriteBytes(u16 WriteAddr,u32 DataToWrite,u8 Len); //指定地址开始写入指定长度的数据
u32  Screen_ReadBytes(u16 ReadAddr,u8 Len);					//指定地址开始读取指定长度数据
void Screen_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void Screen_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据

void Screen_Init(void);   //初始化IIC
void Screen_Config(void);

void Vot_WriteByte(u8 WriteAddr,u8 DataToWrite);
void Vot_WriteByte(u8 WriteAddr,u8 DataToWrite);
//u8 SM_WriteData(u8 slaveAddr,u16 address,u8 *data,u8 len);
u32 SM_ReadData(u8 SlaveAddress, u16 REG_Address, u8 REG_data_len, u8 *dest);
u8 SM_WriteData(u8 slaveAddr,u16 address,u32 data,u8 len);
u8 iic2_uh2d_read8(u16 ReadAddr);

void iic2_uh2d_write8(u16 WriteAddr,u8 DataToWrite);
void iic2_uh2d_write16(u16 WriteAddr,u16 DataToWrite);
void iic2_uh2d_write32(u16 WriteAddr,u32 DataToWrite);

#endif //__SCREEN_IIC_H

















#ifndef __I2C_H
#define	__I2C_H

#include "stm32f10x.h"

/* Slave Addresses defines */
#define SLAVE_ADDRESS (0x3e<<1) //0x7c 
#define TCXXX_ADDRESS 0x1e

//void I2C_Screen_Init(void);
//void I2C_BufferWrite(u8 WriteAddr,u8* pBuffer, u16 NumByteToWrite);
//void I2C_ByteWrite(u8 WriteAddr,u8* pBuffer);
//void I2C_PageWrite(u8 WriteAddr,u8* pBuffer,  u8 NumByteToWrite);
//void I2C_BufferRead(u8 ReadAddr,u8* pBuffer,  u16 NumByteToRead);
//void I2C_WaitEepromStandbyState(void);

#endif // __I2C_H

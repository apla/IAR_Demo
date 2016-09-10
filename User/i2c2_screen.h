#ifndef __I2C2_H
#define	__I2C2_H

#include "stm32f10x.h"

/* Slave Addresses defines */
#define SLAVE_ADDRESS (0x3e<<1) //0x7c 
#define TCXXX_ADDRESS 0x1e

//void I2C2_Screen_Init(void);
//void I2C2_BufferWrite(u8 WriteAddr,u8* pBuffer, u16 NumByteToWrite);
//void I2C2_ByteWrite(u8 WriteAddr,u8* pBuffer);
//void I2C2_PageWrite(u8 WriteAddr,u8* pBuffer,  u8 NumByteToWrite);
//void I2C2_BufferRead(u8 ReadAddr,u8* pBuffer,  u16 NumByteToRead);
//void I2C2_WaitEepromStandbyState(void);

#endif // __I2C2_H

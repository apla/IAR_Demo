#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"

//IO方向设置 
#define SDA2_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=(u32)8<<12;}//1000上拉下拉输入模式
#define SDA2_OUT() {GPIOB->CRH&=0X0FFF0FFF;GPIOB->CRH|=(u32)3<<12;}//推挽输出(50M)

//IO操作函数	 
#define IIC_SCL2    PBout(10) //SCL
#define IIC_SDA2    PBout(11) //SDA	 
#define READ_SDA2   PBin(11)  //输入SDA 

//IIC所有操作函数
void IIC2_Init(void);           //初始化IIC的IO口				 
void IIC2_Start(void);				  //发送IIC开始信号
void IIC2_Stop(void);	  			  //发送IIC停止信号
void IIC2_Send_Byte(u8 txd);			//IIC发送一个字节
u8   IIC2_Read_Byte(unsigned char ack);//IIC读取一个字节
u8   IIC2_Wait_Ack(void); 			//IIC等待ACK信号
void IIC2_Ack(void);					  //IIC发送ACK信号
void IIC2_NAck(void);				    //IIC不发送ACK信号

void IIC2_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8   IIC2_Read_One_Byte(u8 daddr,u8 addr);	  

#endif

















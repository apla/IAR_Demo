#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"

//IO�������� 
#define SDA2_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=(u32)8<<12;}//1000������������ģʽ
#define SDA2_OUT() {GPIOB->CRH&=0X0FFF0FFF;GPIOB->CRH|=(u32)3<<12;}//�������(50M)

//IO��������	 
#define IIC_SCL2    PBout(10) //SCL
#define IIC_SDA2    PBout(11) //SDA	 
#define READ_SDA2   PBin(11)  //����SDA 

//IIC���в�������
void IIC2_Init(void);           //��ʼ��IIC��IO��				 
void IIC2_Start(void);				  //����IIC��ʼ�ź�
void IIC2_Stop(void);	  			  //����IICֹͣ�ź�
void IIC2_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8   IIC2_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8   IIC2_Wait_Ack(void); 			//IIC�ȴ�ACK�ź�
void IIC2_Ack(void);					  //IIC����ACK�ź�
void IIC2_NAck(void);				    //IIC������ACK�ź�

void IIC2_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8   IIC2_Read_One_Byte(u8 daddr,u8 addr);	  

#endif

















/********************************************************************************
 * �ļ���  ��I2C2_screen.c
 * ����    ��i2c ��Ļ����Ӧ�ú�����  
 * Ӳ�����ӣ�-----------------
 *          |                 |
 *          |  PB6-I2C1_SCL	  |
 *          |  PB7-I2C1_SDA   |
 *          |                 |
 *           -----------------
 * ��汾  ��ST3.5.0
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
//Mini STM32?a�����?��1��?��?��?24c02��??����??����?EE_TYPE?aAT24C02
#define EE_TYPE AT24C02
//��ʼ��IIC�ӿ�
void Screen_Init(void)
{
	IIC_Init();
}
//��Screenָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
//�궨���и�ѡ��оƬ���͵ģ��������������AT24C16�Ļ��ͷ�������8bit��ַ������ͷ���һ��8bit��ַ
u8 Screen_ReadByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
        IIC_Start();  
        
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ
		IIC_Wait_Ack();		 
	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 
        

	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //�������ģʽ			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}
//��Screenָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void Screen_WriteByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
        IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ
 	}else
	{
		IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 
	}	 
	IIC_Wait_Ack();	   
        IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack();  		    	   
        IIC_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}
//��Screen�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void Screen_WriteBytes(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		Screen_WriteByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//��Screen�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
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
//���Screen�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 Screen_Check(void)
{
	u8 temp;
	temp=Screen_ReadByte(255);//����ÿ�ο�����дScreen			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		Screen_WriteByte(255,0X55);
	    temp=Screen_ReadByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//��Screen�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void Screen_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=Screen_ReadByte(ReadAddr++);	
		NumToRead--;
	}
}  
//��Screen�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
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

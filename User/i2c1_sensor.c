/********************************************************************************
 * �ļ���  ��i2c_screen.c
 * ����    ��i2c ��Ļ����Ӧ�ú�����  
 * Ӳ�����ӣ�-----------------
 *          |                 |
 *          |  PB6-I2C1_SCL	  |
 *          |  PB7-I2C1_SDA   |
 *          |                 |
 *           -----------------
 * ��汾  ��ST3.5.0
**********************************************************************************/
#include "i2c1_sensor.h"

#define I2C1_Speed              400000
#define I2C1_OWN_ADDRESS7      0x0A
#define I2C1_PageSize           8			/* AT24C02ÿҳ��8���ֽ� */

//#include "MPU.h"
//#include "usart.h"
#include "delay.h"
#include <math.h>    //Keil library  
#include <stdio.h>   //Keil library	
// ����MPU6050�ڲ���ַ
//****************************************
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			  0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)

#define	ACCEL_XOUT_H	0x3B	//0
#define	ACCEL_XOUT_L	0x3C	//1
#define	ACCEL_YOUT_H	0x3D	//2
#define	ACCEL_YOUT_L	0x3E	//3
#define	ACCEL_ZOUT_H	0x3F	//4
#define	ACCEL_ZOUT_L	0x40	//5
#define	TEMP_OUT_H		0x41	//6
#define	TEMP_OUT_L		0x42	//7
#define	GYRO_XOUT_H		0x43	//8
#define	GYRO_XOUT_L		0x44	//9
#define	GYRO_YOUT_H		0x45	//10
#define	GYRO_YOUT_L		0x46	//11
#define	GYRO_ZOUT_H		0x47  //12
#define	GYRO_ZOUT_L		0x48	//13

#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I			0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define	SlaveAddress	0xD0	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ


#if 0
//**************************************
//��I2C�豸��ȡһ���ֽ�����
//**************************************
u8 MPU6050_ReadI2C(u8 REG_Address)
{
	u8 REG_data;
	IIC_Start();                  //��ʼ�ź�
	IIC_Send_Byte(SlaveAddress);  //�����豸��ַ+д�ź�
	REG_data=IIC_Wait_Ack();	   
	IIC_Send_Byte(REG_Address);   //���ʹ洢��Ԫ��ַ����0��ʼ
	REG_data=IIC_Wait_Ack();	   
	IIC_Start();                  //��ʼ�ź�
	IIC_Send_Byte(SlaveAddress+1);//�����豸��ַ+���ź�
	REG_data=IIC_Wait_Ack();	   
  REG_data=IIC_Read_Byte(0);		//��ȡһ���ֽ�,�������ٶ�,����NAK,�����Ĵ�������
	IIC_Stop();	                  //ֹͣ�ź�
	return REG_data;
}
//**************************************
//��I2C�豸д��һ���ֽ�����
//**************************************
void MPU6050_WriteI2C(u8 REG_Address,u8 REG_data)
{
    IIC_Start();                   //��ʼ�ź�
    IIC_Send_Byte(SlaveAddress);   //�����豸��ַ+д�ź�
		IIC_Wait_Ack();	   
    IIC_Send_Byte(REG_Address);    //�ڲ��Ĵ�����ַ
		IIC_Wait_Ack(); 	 										  		   
    IIC_Send_Byte(REG_data);       //�ڲ��Ĵ�������
		IIC_Wait_Ack(); 	 										  		   
    IIC_Stop();                    //����ֹͣ�ź�
}
//**************************************
//��ʼ��MPU6050
//**************************************
void MPU6050_Init(void)
{
	u8 Add;//������ַ
	IIC_Init();//��ʼ��IIC�˿�
	Add=MPU6050_ReadI2C(WHO_AM_I);
	//printf("ID=%x\r\n",Add);
	MPU6050_WriteI2C(PWR_MGMT_1, 0x00);	//�������״̬
	MPU6050_WriteI2C(SMPLRT_DIV, 0x07);
	MPU6050_WriteI2C(CONFIG, 0x06);
	MPU6050_WriteI2C(GYRO_CONFIG, 0x18);
        MPU6050_WriteI2C(ACCEL_CONFIG, 0x08);	//Bit4 3 
                                                //0  �� 2g		     16384	���ٶ�
                                                //1  �� 4g	       8192
                                                //2  �� 8g	       4096
                                                //3  �� 16g	     2048
                                                //0  �� 250 ��/s	 131	  ������
                                                //1  �� 500 ��/s	 65.5
                                                //2  �� 1000��/s	 32.8
                                                //3  �� 2000��/s	 16.4
}
//��ȡ3���������
//x,y,z:��ȡ��������
void MPU6050_RD_XYZ(MPU_TypeDef* Mympu)
{
	u8 buf[14];
	u8 i;
	IIC_Start();  				 
	IIC_Send_Byte(SlaveAddress);	//����д����ָ��	 
	IIC_Wait_Ack();	   
	IIC_Send_Byte(0x3B);   		//���ͼĴ�����ַ(���ݻ������ʼ��ַΪ0X3B)
	IIC_Wait_Ack(); 	 										  		   
 	IIC_Start();  	 	   		//��������
	IIC_Send_Byte(SlaveAddress+1);	//���Ͷ�����ָ��
	IIC_Wait_Ack();
	for(i=0;i<14;i++)
	{
		if(i==13)buf[i]=IIC_Read_Byte(0);//��ȡһ���ֽ�,�������ٶ�,����NACK  
		else buf[i]=IIC_Read_Byte(1);	//��ȡһ���ֽ�,������,����ACK 
 	}	        	   
  IIC_Stop();					//����һ��ֹͣ����
	Mympu->Ax=(short)(((u16)buf[0]<<8)+buf[1]); 
	Mympu->Ay=(short)(((u16)buf[2]<<8)+buf[3]); 
	Mympu->Az=(short)(((u16)buf[4]<<8)+buf[5]); 
	Mympu->Tt=(short)(((u16)buf[6]<<8)+buf[7]);
	Mympu->Gx=(short)(((u16)buf[8]<<8)+buf[9]); 
	Mympu->Gy=(short)(((u16)buf[10]<<8)+buf[11]); 
	Mympu->Gz=(short)(((u16)buf[12]<<8)+buf[13]); 
}
//��ȡADXL��ƽ��ֵ
//x,y,z:��ȡ10�κ�ȡƽ��ֵ
void MPU6050_RD_Avval(MPU_TypeDef* Mympu)
{
	u8 i;  
	MPU_TypeDef tMympu;   
	for(i=0;i<10;i++)
	{
		MPU6050_RD_XYZ(Mympu);
		delay_ms(10);
		tMympu.Ax+=(short)Mympu->Ax;
		tMympu.Ay+=(short)Mympu->Ay;
		tMympu.Az+=(short)Mympu->Az;	   
	}
	Mympu->Ax=tMympu.Ax/10;
	Mympu->Ay=tMympu.Ay/10;
	Mympu->Az=tMympu.Az/10;
} 
//��ȡADXL345������times��,��ȡƽ��
//x,y,z:����������
//times:��ȡ���ٴ�
void MPU6050_Read_Average(MPU_TypeDef* Mympu,u8 times)
{
	u8 i;
	MPU_TypeDef tMympu;   
  Mympu->Ax=0;
  Mympu->Ay=0;
	Mympu->Az=0;
	if(times)//��ȡ������Ϊ0
	{
		for(i=0;i<times;i++)//������ȡtimes��
		{
			MPU6050_RD_XYZ(&tMympu);
			Mympu->Ax+=(short)tMympu.Ax;
			Mympu->Ay+=(short)tMympu.Ay;
			Mympu->Az+=(short)tMympu.Az;	   
			delay_ms(5);
		}
		Mympu->Ax/=times;
		Mympu->Ay/=times;
		Mympu->Az/=times;
	}
}
//�õ��Ƕ�
//x,y,z:x,y,z������������ٶȷ���(����Ҫ��λ,ֱ����ֵ����)
//dir:Ҫ��õĽǶ�.0,��Z��ĽǶ�;1,��X��ĽǶ�;2,��Y��ĽǶ�.
//����ֵ:�Ƕ�ֵ.��λ0.1��.
short MPU6050_Get_Angle(float x,float y,float z,u8 dir)
{
	float temp;
 	float res=0;
	switch(dir)
	{
		case 0://����ȻZ��ĽǶ�
		temp=sqrt((x*x+y*y))/z;
 			res=atan(temp);
 			break;
		case 1://����ȻX��ĽǶ�
 			temp=x/sqrt((y*y+z*z));
 			res=atan(temp);
 			break;
 		case 2://����ȻY��ĽǶ�
 			temp=y/sqrt((x*x+z*z));
 			res=atan(temp);
 			break;
 	}
	return res*1800/3.14;//�ѻ���ת���ɽǶ�
}
#endif
//-----------------------------------------------------------------------------



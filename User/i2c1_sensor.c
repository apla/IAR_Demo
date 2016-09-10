/********************************************************************************
 * 文件名  ：i2c_screen.c
 * 描述    ：i2c 屏幕配置应用函数库  
 * 硬件连接：-----------------
 *          |                 |
 *          |  PB6-I2C1_SCL	  |
 *          |  PB7-I2C1_SDA   |
 *          |                 |
 *           -----------------
 * 库版本  ：ST3.5.0
**********************************************************************************/
#include "i2c1_sensor.h"

#define I2C1_Speed              400000
#define I2C1_OWN_ADDRESS7      0x0A
#define I2C1_PageSize           8			/* AT24C02每页有8个字节 */

//#include "MPU.h"
//#include "usart.h"
#include "delay.h"
#include <math.h>    //Keil library  
#include <stdio.h>   //Keil library	
// 定义MPU6050内部地址
//****************************************
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			  0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)

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

#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I			0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0xD0	//IIC写入时的地址字节数据，+1为读取


#if 0
//**************************************
//从I2C设备读取一个字节数据
//**************************************
u8 MPU6050_ReadI2C(u8 REG_Address)
{
	u8 REG_data;
	IIC_Start();                  //起始信号
	IIC_Send_Byte(SlaveAddress);  //发送设备地址+写信号
	REG_data=IIC_Wait_Ack();	   
	IIC_Send_Byte(REG_Address);   //发送存储单元地址，从0开始
	REG_data=IIC_Wait_Ack();	   
	IIC_Start();                  //起始信号
	IIC_Send_Byte(SlaveAddress+1);//发送设备地址+读信号
	REG_data=IIC_Wait_Ack();	   
  REG_data=IIC_Read_Byte(0);		//读取一个字节,不继续再读,发送NAK,读出寄存器数据
	IIC_Stop();	                  //停止信号
	return REG_data;
}
//**************************************
//向I2C设备写入一个字节数据
//**************************************
void MPU6050_WriteI2C(u8 REG_Address,u8 REG_data)
{
    IIC_Start();                   //起始信号
    IIC_Send_Byte(SlaveAddress);   //发送设备地址+写信号
		IIC_Wait_Ack();	   
    IIC_Send_Byte(REG_Address);    //内部寄存器地址
		IIC_Wait_Ack(); 	 										  		   
    IIC_Send_Byte(REG_data);       //内部寄存器数据
		IIC_Wait_Ack(); 	 										  		   
    IIC_Stop();                    //发送停止信号
}
//**************************************
//初始化MPU6050
//**************************************
void MPU6050_Init(void)
{
	u8 Add;//器件地址
	IIC_Init();//初始化IIC端口
	Add=MPU6050_ReadI2C(WHO_AM_I);
	//printf("ID=%x\r\n",Add);
	MPU6050_WriteI2C(PWR_MGMT_1, 0x00);	//解除休眠状态
	MPU6050_WriteI2C(SMPLRT_DIV, 0x07);
	MPU6050_WriteI2C(CONFIG, 0x06);
	MPU6050_WriteI2C(GYRO_CONFIG, 0x18);
        MPU6050_WriteI2C(ACCEL_CONFIG, 0x08);	//Bit4 3 
                                                //0  ± 2g		     16384	加速度
                                                //1  ± 4g	       8192
                                                //2  ± 8g	       4096
                                                //3  ± 16g	     2048
                                                //0  ± 250 °/s	 131	  陀螺仪
                                                //1  ± 500 °/s	 65.5
                                                //2  ± 1000°/s	 32.8
                                                //3  ± 2000°/s	 16.4
}
//读取3个轴的数据
//x,y,z:读取到的数据
void MPU6050_RD_XYZ(MPU_TypeDef* Mympu)
{
	u8 buf[14];
	u8 i;
	IIC_Start();  				 
	IIC_Send_Byte(SlaveAddress);	//发送写器件指令	 
	IIC_Wait_Ack();	   
	IIC_Send_Byte(0x3B);   		//发送寄存器地址(数据缓存的起始地址为0X3B)
	IIC_Wait_Ack(); 	 										  		   
 	IIC_Start();  	 	   		//重新启动
	IIC_Send_Byte(SlaveAddress+1);	//发送读器件指令
	IIC_Wait_Ack();
	for(i=0;i<14;i++)
	{
		if(i==13)buf[i]=IIC_Read_Byte(0);//读取一个字节,不继续再读,发送NACK  
		else buf[i]=IIC_Read_Byte(1);	//读取一个字节,继续读,发送ACK 
 	}	        	   
  IIC_Stop();					//产生一个停止条件
	Mympu->Ax=(short)(((u16)buf[0]<<8)+buf[1]); 
	Mympu->Ay=(short)(((u16)buf[2]<<8)+buf[3]); 
	Mympu->Az=(short)(((u16)buf[4]<<8)+buf[5]); 
	Mympu->Tt=(short)(((u16)buf[6]<<8)+buf[7]);
	Mympu->Gx=(short)(((u16)buf[8]<<8)+buf[9]); 
	Mympu->Gy=(short)(((u16)buf[10]<<8)+buf[11]); 
	Mympu->Gz=(short)(((u16)buf[12]<<8)+buf[13]); 
}
//读取ADXL的平均值
//x,y,z:读取10次后取平均值
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
//读取ADXL345的数据times次,再取平均
//x,y,z:读到的数据
//times:读取多少次
void MPU6050_Read_Average(MPU_TypeDef* Mympu,u8 times)
{
	u8 i;
	MPU_TypeDef tMympu;   
  Mympu->Ax=0;
  Mympu->Ay=0;
	Mympu->Az=0;
	if(times)//读取次数不为0
	{
		for(i=0;i<times;i++)//连续读取times次
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
//得到角度
//x,y,z:x,y,z方向的重力加速度分量(不需要单位,直接数值即可)
//dir:要获得的角度.0,与Z轴的角度;1,与X轴的角度;2,与Y轴的角度.
//返回值:角度值.单位0.1°.
short MPU6050_Get_Angle(float x,float y,float z,u8 dir)
{
	float temp;
 	float res=0;
	switch(dir)
	{
		case 0://与自然Z轴的角度
		temp=sqrt((x*x+y*y))/z;
 			res=atan(temp);
 			break;
		case 1://与自然X轴的角度
 			temp=x/sqrt((y*y+z*z));
 			res=atan(temp);
 			break;
 		case 2://与自然Y轴的角度
 			temp=y/sqrt((x*x+z*z));
 			res=atan(temp);
 			break;
 	}
	return res*1800/3.14;//把弧度转换成角度
}
#endif
//-----------------------------------------------------------------------------



#include "mpu6500.h"
#include "sys.h"
#include "delay.h"
 
 u8 InReport[]={//62
   // 8 byte
   0x01,      //ready                      
   0x01,      //sample count
   0xe8,0x30, //time code
   0x00,0x00, //command id
   0x1c,0x0c, //temperature
   
   // 6* 4= 24 byte
   0xff,0xc1,0x30,0x5d,0xe5,0xc0, // Accel 1   [8] to  [13]
   
   0x16,0xe0,0x00,0x00,0xc0,0x00, // Accel 2   [14] to [19]
   0x0b,0x00,0x01,0x02,0xff,0xa6, // Accel 3   [20] to [25]
   0x40,0x5c,0xe2,0x00,0x14,0x8e, // Accel 4   [26] to [31]
   
   // 6* 4= 24 byte
   0xff,0xff,0xc8,0x00,0x0b,0x40, //  陀螺仪 [32] to [37] 
   0x00,0xd6,0x00,0x00,0x00,0x00,
   0x00,0x00,0x00,0x00,0x00,0x00,
   0x00,0x00,0x00,0x00,0x00,0x00,
   // 6 byte 
   0x00,0x00, 0x00,0x00, 0x00,0x00// 地磁
  };

void MPU6500_Init(void)
{	
    
        GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB CLK
        //PB12 SENSOR_MPU_SPI_CSN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);
        
        MPU6500_CS = 1;//不选中				
	SPI2_Init();		   	
	//SPI2_SetSpeed(SPI_BaudRatePrescaler_2);
        
	MPU_ReadID();        
} 

u8 MPU_ReadID(void)
{
        u8 res,addr = MPU_DEVICE_ID_REG; 
	//MPU6500_CS = 0;
        GPIO_ResetBits(GPIOB,GPIO_Pin_12);
        addr |= 0x80;
	SPI2_ReadWriteByte(addr); 	//	(MPU_DEVICE_ID_REG | (1<<7)); 
        res = SPI2_ReadWriteByte(0xFF);
        GPIO_SetBits(GPIOB,GPIO_Pin_12);
	if(res == MPU_6500_ID)//器件ID正确
	{
              // reset MPU
              MPU_WriteReg(0x6b,0x80); //reset
              delay_ms(100);
              MPU_WriteReg(0x68,0x3); //reset GYTO ACCRL TEMP
              MPU_WriteReg(0x6a,0x1); //reset GYTO ACCRL TEMP

              MPU_WriteReg(0x68,0x0); //reset GYTO ACCRL TEMP
              MPU_WriteReg(0x6a,0x0); //reset GYTO ACCRL TEMP
              delay_ms(100);
              MPU_Config();
             
 	}else return 0;
	GPIO_SetBits(GPIOB,GPIO_Pin_12);			    
	return res;
} 

void MPU_Config(void)
{
     // === fast config
     u8 Reg_Addr[8] ={
                    0x19, //SMPLRT_DIV
                    0x1A, //CONFIG 
                    0x1B, //GYRO_CONFIG
                    0x1C, //ACCEL_CONFIG
                    0x1D, //ACCEL_CONFIG 2
                    0x6A, //USER_CTRL
                    0x6B, //PWR_MGMT_1
                    0x6C };//PWR_MGMT_2
                    
    u8  Reg_Val[8] ={
                    0x07, //0x01, //SAMPLE_RATE
                    0x00, //Gyro Fs = 32KHz
		    0x18,// 0x1a,// 0x18,//csn change ;Gyro Full Scale = +-2000dps
                    0x18,// 0x08,  //0x18,//+-16G // 0x08,//+-4G // 0x18, //Acc Full Scale = +-16G
                    0x08, //Acc Rate = 4KHz
                    0x10, //SPI mode only.
                    0x00, //PLL if ready, else use the Internal oscillator
                    0x00 };//All sensors on
    u8 i=0;
    for(i = 1;i < 8; i++)
    {
        MPU_WriteReg(Reg_Addr[i],Reg_Val[i]); 
        delay_ms(10);
    }
}

void MPU_WriteReg(u8 WriteAddr, u8 WriteData)
{
      MPU6500_CS = 0;
      SPI2_ReadWriteByte(WriteAddr);//SSP_writeByte
      SPI2_ReadWriteByte(WriteData);
      MPU6500_CS = 1;
}

void MPU_ReadData(int *accData,int *gyroData)
{
    u8 bufAR[14];
 //   u16  Temperature_sum = 0;
 //   float accx,accy,accz = 0;
 //   float gyrox,gyroy,gyroz = 0;
//    float ypr[3]; // yaw pitch roll
    u16 timestamp = 0;    
          
 //   float accdatabuf1[3] = {0};   
    MPU_ReadBuf( 0x3B , bufAR , 14 );
  
    // time 
 //   timestamp =  VirtualTimer_now();TODO
    InReport[2] = ((short)(timestamp) >> 8) & 0xFF;
    InReport[3] = (short)(timestamp) & 0xFF;;
    
    //temperatur
    InReport[6] = bufAR[6];
    InReport[7] = bufAR[7];
    
    // acc
    InReport[8] = bufAR[0];
    InReport[9] = bufAR[1];
    InReport[10] = bufAR[2];
    InReport[11] = bufAR[3];
    InReport[12] = bufAR[4];
    InReport[13] = bufAR[5];
    //gyro
    InReport[32]  = bufAR[8];
    InReport[33]  = bufAR[9];
    InReport[34] = bufAR[10];
    InReport[35] = bufAR[11];
    InReport[36] = bufAR[12];
    InReport[37] = bufAR[13];
    
    //mag
    InReport[56]  = InReport[56];
    InReport[57]  = InReport[57];
    InReport[58]  = InReport[58] ;
    InReport[59]  = InReport[59];
    InReport[60]  = InReport[60];
    InReport[61]  = InReport[61];   
	
}

void MPU_ReadBuf( u8 ReadAddr, u8 *ReadBuf, u8 Bytes )
{
    u8 i = 0;
    MPU6500_CS = 0;
    //The first bit of the first byte:Read(1) Write(0)
    ReadAddr |= 0x80;
    SPI2_ReadWriteByte(ReadAddr);//SSP_writeByte
    //TODO
    for(i=0; i<Bytes; i++)
	    ReadBuf[i] = SPI2_ReadWriteByte(0xFF);//SSP_readByte(&ReadBuf[i])
    MPU6500_CS = 1;
	
}
/////////////////////////////////////////////////////////
//初始化MPU6050  #define	CS 		PBout(12)  		//cs
//返回值:0,成功  
//    其他,错误代码
u8 MPU_Init(void)
{ 
	u8 res;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//使能AFIO时钟 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//先使能外设IO PORTA时钟 
	
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	 // 端口配置
//        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
//        GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA
//
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//禁止JTAG,从而PA15可以做普通IO使用,否则PA15不能做普通IO!!!
	
//	MPU_AD0_CTRL=0;			//控制MPU6050的AD0脚为低电平,从机地址为:0X68
	
	MPU_IIC_Init();//初始化IIC总线
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//复位MPU6050
        delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//唤醒MPU6050 
	MPU_Set_Gyro_Fsr(3);					//陀螺仪传感器,±2000dps
	MPU_Set_Accel_Fsr(0);					//加速度传感器,±2g
	MPU_Set_Rate(50);						//设置采样率50Hz
	MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//关闭所有中断
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);	//关闭FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if(res==MPU_ADDR)//器件ID正确
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
		MPU_Set_Rate(50);						//设置采样率为50Hz
 	}else return 1;
	return 0;
}
//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}
//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_CFG_REG,data);//设置数字低通滤波器  
}
//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}

//得到温度值
//返回值:温度值(扩大了100倍)
short MPU_Get_Temperature(void)
{
    u8 buf[2]; 
    short raw;
    float temp;
    MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return (short)(temp*100);
}
//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;
}
//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
    res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
    if(res==0)
    {
            *ax=((u16)buf[0]<<8)|buf[1];  
            *ay=((u16)buf[2]<<8)|buf[3];  
            *az=((u16)buf[4]<<8)|buf[5];
    } 	
    return res;
}
//IIC连续写
//addr:器件地址 
//reg:寄存器地址
//len:写入长度
//buf:数据区
//返回值:0,正常
//    其他,错误代码
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
        MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((addr<<1)|0);//发送器件地址+写命令	
	if(MPU_IIC_Wait_Ack())	//等待应答
	{
		MPU_IIC_Stop();		 
		return 1;		
	}
        MPU_IIC_Send_Byte(reg);	//写寄存器地址
        MPU_IIC_Wait_Ack();		//等待应答
	for(i=0;i<len;i++)
	{
		MPU_IIC_Send_Byte(buf[i]);	//发送数据
		if(MPU_IIC_Wait_Ack())		//等待ACK
		{
			MPU_IIC_Stop();	 
			return 1;		 
		}		
	}    
        MPU_IIC_Stop();	 
	return 0;	
} 
//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((addr<<1)|0);//发送器件地址+写命令	
	if(MPU_IIC_Wait_Ack())	//等待应答
	{
		MPU_IIC_Stop();		 
		return 1;		
	}
        MPU_IIC_Send_Byte(reg);	//写寄存器地址
        MPU_IIC_Wait_Ack();		//等待应答
        MPU_IIC_Start();
 	MPU_IIC_Send_Byte((addr<<1)|1);//发送器件地址+读命令	
        MPU_IIC_Wait_Ack();		//等待应答 
	while(len)
	{
		if(len==1)*buf=MPU_IIC_Read_Byte(0);//读数据,发送nACK 
		else *buf=MPU_IIC_Read_Byte(1);		//读数据,发送ACK  
		len--;
		buf++; 
	}    
        MPU_IIC_Stop();	//产生一个停止条件 
	return 0;	
}
//IIC写一个字节 
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
u8 MPU_Write_Byte(u8 reg,u8 data) 				 
{ 
        MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//发送器件地址+写命令	
	if(MPU_IIC_Wait_Ack())	//等待应答
	{
		MPU_IIC_Stop();		 
		return 1;		
	}
        MPU_IIC_Send_Byte(reg);	//写寄存器地址
        MPU_IIC_Wait_Ack();		//等待应答 
	MPU_IIC_Send_Byte(data);//发送数据
	if(MPU_IIC_Wait_Ack())	//等待ACK
	{
		MPU_IIC_Stop();	 
		return 1;		 
	}		 
        MPU_IIC_Stop();	 
	return 0;
}
//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
        MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//发送器件地址+写命令	
	MPU_IIC_Wait_Ack();		//等待应答 
        MPU_IIC_Send_Byte(reg);	//写寄存器地址
        MPU_IIC_Wait_Ack();		//等待应答
        MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|1);//发送器件地址+读命令	
        MPU_IIC_Wait_Ack();		//等待应答 
	res=MPU_IIC_Read_Byte(0);//读取数据,发送nACK 
        MPU_IIC_Stop();			//产生一个停止条件 
	return res;		
}

/*************************END OF FILE*************************************/

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

//uint16_t EEPROM_ADDRESS;

/*
 * ��������I2C2_GPIO_Config
 * ����  ��I2C1 I/O����
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void I2C2_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

  /* ʹ���� I2C1 �йص�ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
  //i2c1_sensor
  /* PB6-I2C1_SCL��PB7-I2C1_SDA*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // ��©���
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //I2C2_screen
  /* ʹ���� I2C2 �йص�ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
    
  /* PB10-I2C2_SCL��PB11-I2C2_SDA*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // ��©���
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
}

/*
 * ��������I2C2_Configuration
 * ����  ��I2C ����ģʽ����
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void I2C2_Mode_Configu(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C ���� */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 =I2C2_OWN_ADDRESS7; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C2_Speed;
  
  /* ʹ�� I2C2,I2C2 */
  I2C_Cmd(I2C2, ENABLE);
  I2C_Cmd(I2C2, ENABLE);
  /* I2C2,I2C2 ��ʼ�� */
  I2C_Init(I2C2, &I2C_InitStructure);
  I2C_Init(I2C2, &I2C_InitStructure);
  
}

/*
 * ��������I2C2_Init
 * ����  ��I2C ����(EEPROM)��ʼ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void I2C2_Screen_Init(void)
{

  I2C2_GPIO_Config(); 
 
  I2C2_Mode_Configu();

}


/*************************END OF FILE*************************************/

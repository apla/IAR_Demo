/********************************************************************************
 * 文件名  ：I2C2_screen.c
 * 描述    ：i2c 屏幕配置应用函数库  
 * 硬件连接：-----------------
 *          |                 |
 *          |  PB6-I2C1_SCL	  |
 *          |  PB7-I2C1_SDA   |
 *          |                 |
 *           -----------------
 * 库版本  ：ST3.5.0
**********************************************************************************/
#include "I2C2_screen.h"

#define I2C2_Speed              400000
#define I2C2_OWN_ADDRESS7      0x0A
#define I2C2_PageSize           8	

//uint16_t EEPROM_ADDRESS;

/*
 * 函数名：I2C2_GPIO_Config
 * 描述  ：I2C1 I/O配置
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void I2C2_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

  /* 使能与 I2C1 有关的时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
  //i2c1_sensor
  /* PB6-I2C1_SCL、PB7-I2C1_SDA*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // 开漏输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //I2C2_screen
  /* 使能与 I2C2 有关的时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
    
  /* PB10-I2C2_SCL、PB11-I2C2_SDA*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // 开漏输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
}

/*
 * 函数名：I2C2_Configuration
 * 描述  ：I2C 工作模式配置
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void I2C2_Mode_Configu(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C 配置 */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 =I2C2_OWN_ADDRESS7; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C2_Speed;
  
  /* 使能 I2C2,I2C2 */
  I2C_Cmd(I2C2, ENABLE);
  I2C_Cmd(I2C2, ENABLE);
  /* I2C2,I2C2 初始化 */
  I2C_Init(I2C2, &I2C_InitStructure);
  I2C_Init(I2C2, &I2C_InitStructure);
  
}

/*
 * 函数名：I2C2_Init
 * 描述  ：I2C 外设(EEPROM)初始化
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void I2C2_Screen_Init(void)
{

  I2C2_GPIO_Config(); 
 
  I2C2_Mode_Configu();

}


/*************************END OF FILE*************************************/

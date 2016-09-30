#include "screen_iic.h" 
#include "delay.h"
#include "Constants.h"
#define  NULL 0
#define  Vot_Slave_Address (0x3e<<1) //0x7C(write) ox7D(read)
#define  TCXXX_ADDRESS 0x1e  // TC358870XBG iic slave address :0x0F 
u8 HDMI_PLUG = 0;
//初始化IIC接口
void Screen_Init(void)
{
      u8 i;
      GPIO_InitTypeDef GPIO_InitStructure;
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);	
      GPIO_InitStructure.GPIO_Pin = TC_RESET_L; 
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin = LCD0_BL_EN | LCD0_BIAS_PEN | LCD0_BIAS_NEN ; 
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin = MCU_OLED_0_RST | MCU_OLED_1_RST; 
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
      IIC2_Init();//初始化i2c端口 PB10,PB11
      //Reset ( TC_RESET_L ) PB1      
      GPIO_ResetBits(GPIOB, TC_RESET_L);//TC_RESET_L
      delay_ms(2);
      GPIO_SetBits(GPIOB, TC_RESET_L);
      delay_ms(10+20);       
      //LCD背光 ( LCD0_BL_EN ) PA3,6,7   
      GPIO_ResetBits(GPIOA,LCD0_BL_EN);//背光 LCD0_BL_EN
      delay_ms(3);
      for(i=0;i<(20);i++){  //1-32(less-)          after+32-front  (32+32-1);20;
         GPIO_ResetBits(GPIOA,LCD0_BL_EN);
         delay_us(1);
         GPIO_SetBits(GPIOA,LCD0_BL_EN);
         delay_us(1);
      }
      //MCU_OLED_1_RST(PC13) ,MCU_OLED_0_RST(PC14)
      delay_ms(40);
      GPIO_SetBits(GPIOA, LCD0_BIAS_PEN);//LCD0_BIAS_PEN PA6_0x40010800
      delay_ms(4);
      GPIO_SetBits(GPIOA, LCD0_BIAS_NEN);//LCD0_BIAS_NEN PA7_0x40010800
      delay_ms(15);
      //vot_set:5.8v,-5.6v  
      delay_ms(15);      
      Vot_WriteByte(0x01, 0x10);//LCM_AVEE: -5.6v 
      Vot_WriteByte(0x00, 0x12);//LCM_AVDD:  5.8v   
      delay_ms(15);       
      //PANEL_GPIO_RESET_PIN
      GPIO_SetBits(GPIOC, MCU_OLED_0_RST);
      GPIO_SetBits(GPIOC, MCU_OLED_1_RST);        
      delay_ms(1);
      GPIO_ResetBits(GPIOC, MCU_OLED_0_RST);
      GPIO_ResetBits(GPIOC, MCU_OLED_1_RST);               
      delay_ms(1);
      GPIO_SetBits(GPIOC, MCU_OLED_0_RST);
      GPIO_SetBits(GPIOC, MCU_OLED_1_RST);   
      //start lcd init...
    //  Screen_Config_1920_1080();
      sharp_lin_1440_2560();
}
void PANEL_GPIO_RESET_PIN(u8 flag)
{
	//SM_UART_PutStr(USART1, "AUO PANEL RESET SIGNAL\r\n");
	if(flag == 1){
#if 0       
		//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		//GPIO_ResetBits(GPIOC, GPIO_Pin_14);
		//SmDelayms(1);
                   SmDelayms(1);
		//GPIO_SetBits(GPIOC, GPIO_Pin_13);
		//GPIO_SetBits(GPIOC, GPIO_Pin_14);
#else               
                GPIO_SetBits(GPIOC, GPIO_Pin_13);
		GPIO_SetBits(GPIOC, GPIO_Pin_14);
                SmDelayms(1);
                GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		GPIO_ResetBits(GPIOC, GPIO_Pin_14);
                SmDelayms(1);
                GPIO_SetBits(GPIOC, GPIO_Pin_13);
		GPIO_SetBits(GPIOC, GPIO_Pin_14);
#endif
		SmDelayms(5);
                 // SmDelayus(1000);
                 // VirtualTimer_sleep(5);
	}
	if(flag == 0){
          
                  // GPIO_clear(LOGICAL_GPIO_OLED_0_RST_OUT);
                  // GPIO_clear(LOGICAL_GPIO_OLED_1_RST_OUT); 
		   GPIO_ResetBits(GPIOC, GPIO_Pin_13);//MCU_OLED_0_RST
		   GPIO_ResetBits(GPIOC, GPIO_Pin_14);//MCU_OLED_1_RST
	}
}

void Screen_Config_1920_1080(void)//1080*1920
{
        u32 cbit = 0;     
      // Software Reset
	 SM_WriteData(TCXXX_ADDRESS,0x0004,0x0004,2); // ConfCtl0
        SM_WriteData(TCXXX_ADDRESS,0x0002,0x3F01,2); // SysCtl
        SM_WriteData(TCXXX_ADDRESS,0x0002,0x0000,2); // SysCtl
        SM_WriteData(TCXXX_ADDRESS,0x0006,0x0008,2); // ConfCtl1
        // DSI-TX0 Transition Timing
        SM_WriteData(TCXXX_ADDRESS,0x0108,0x00000001,4); // DSI_TX_CLKEN
        SM_WriteData(TCXXX_ADDRESS,0x010C,0x00000001,4); // DSI_TX_CLKSEL
        SM_WriteData(TCXXX_ADDRESS,0x02A0,0x00000001,4); // MIPI_PLL_CONTROL
        SM_WriteData(TCXXX_ADDRESS,0x02AC,0x0000304F,4); // MIPI_PLL_CNF
        SmDelayms(1);
        SM_WriteData(TCXXX_ADDRESS,0x02A0,0x00000003,4); // MIPI_PLL_CONTROL
        SM_WriteData(TCXXX_ADDRESS,0x0118,0x00000014,4); // LANE_ENABLE
        SM_WriteData(TCXXX_ADDRESS,0x0120,0x00001770,4); // LINE_INIT_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0124,0x00000000,4); // HSTX_TO_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0128,0x00000101,4); // FUNC_ENABLE
        SM_WriteData(TCXXX_ADDRESS,0x0130,0x00010000,4); // DSI_TATO_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0134,0x00005000,4); // DSI_PRESP_BTA_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0138,0x00010000,4); // DSI_PRESP_LPR_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x013C,0x00010000,4); // DSI_PRESP_LPW_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0140,0x00010000,4); // DSI_PRESP_HSR_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0144,0x00010000,4); // DSI_PRESP_HSW_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0148,0x00001000,4); // DSI_PR_TO_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x014C,0x00010000,4); // DSI_LRX-H_TO_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0150,0x00000141,4); // FUNC_MODE
        SM_WriteData(TCXXX_ADDRESS,0x0154,0x00000001,4); // DSI_RX_VC_ENABLE
        SM_WriteData(TCXXX_ADDRESS,0x0158,0x000000C8,4); // IND_TO_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0168,0x0000002A,4); // DSI_HSYNC_STOP_COUNT 
        SM_WriteData(TCXXX_ADDRESS,0x0170,0x000003DA,4);
        SM_WriteData(TCXXX_ADDRESS,0x017C,0x00000081,4); // DSI_TX_MODE
        SM_WriteData(TCXXX_ADDRESS,0x018C,0x00000001,4); // DSI_HSYNC_WIDTH
        SM_WriteData(TCXXX_ADDRESS,0x0190,0x000000DF,4); // DSI_HBPR
        SM_WriteData(TCXXX_ADDRESS,0x01A4,0x00000000,4); // DSI_RX_STATE_INT_MASK
        SM_WriteData(TCXXX_ADDRESS,0x01C0,0x00000015,4); // DSI_LPRX_THRESH_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0214,0x00000000,4); // APP_SIDE_ERR_INT_MASK
        SM_WriteData(TCXXX_ADDRESS,0x021C,0x00000080,4); // DSI_RX_ERR_INT_MASK
        SM_WriteData(TCXXX_ADDRESS,0x0224,0x00000000,4); // DSI_LPTX_INT_MASK
        SM_WriteData(TCXXX_ADDRESS,0x0254,0x00000008,4); // LPTXTIMECNT
        SM_WriteData(TCXXX_ADDRESS,0x0258,0x00240007,4); // TCLK_HEADERCNT
        SM_WriteData(TCXXX_ADDRESS,0x025C,0x000D0008,4); // TCLK_TRAILCNT
        SM_WriteData(TCXXX_ADDRESS,0x0260,0x000D0008,4); // THS_HEADERCNT
        SM_WriteData(TCXXX_ADDRESS,0x0264,0x00003E80,4); // TWAKEUPCNT
        SM_WriteData(TCXXX_ADDRESS,0x0268,0x0000000D,4); // TCLK_POSTCNT
        SM_WriteData(TCXXX_ADDRESS,0x026C,0x000D0008,4); // THS_TRAILCNT
        SM_WriteData(TCXXX_ADDRESS,0x0270,0x00000020,4); // HSTXVREGCNT
        SM_WriteData(TCXXX_ADDRESS,0x0274,0x0000001F,4); // HSTXVREGEN
        SM_WriteData(TCXXX_ADDRESS,0x0278,0x00080008,4); // BTA_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x027C,0x00000002,4); // DPHY_TX ADJUST
        SM_WriteData(TCXXX_ADDRESS,0x011C,0x00000001,4); // DSITX_START
        // DSI-TX1 Transition Timing
        SM_WriteData(TCXXX_ADDRESS,0x0308,0x00000001,4); // DSI_TX_CLKEN
        SM_WriteData(TCXXX_ADDRESS,0x030C,0x00000001,4); // DSI_TX_CLKSEL
        SM_WriteData(TCXXX_ADDRESS,0x04A0,0x00000001,4); // MIPI_PLL_CONTROL
        SM_WriteData(TCXXX_ADDRESS,0x04AC,0x0000304F,4); // MIPI_PLL_CNF
        SmDelayms(1);
        SM_WriteData(TCXXX_ADDRESS,0x04A0,0x00000003,4); // MIPI_PLL_CONTROL
        SM_WriteData(TCXXX_ADDRESS,0x0318,0x00000014,4); // LANE_ENABLE
        SM_WriteData(TCXXX_ADDRESS,0x0320,0x00001770,4); // LINE_INIT_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0324,0x00000000,4); // HSTX_TO_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0328,0x00000101,4); // FUNC_ENABLE
        SM_WriteData(TCXXX_ADDRESS,0x0330,0x00010000,4); // DSI_TATO_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0334,0x00005000,4); // DSI_PRESP_BTA_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0338,0x00010000,4); // DSI_PRESP_LPR_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x033C,0x00010000,4); // DSI_PRESP_LPW_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0340,0x00010000,4); // DSI_PRESP_HSR_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0344,0x00010000,4); // DSI_PRESP_HSW_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0348,0x00001000,4); // DSI_PR_TO_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x034C,0x00010000,4); // DSI_LRX-H_TO_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0350,0x00000141,4); // FUNC_MODE
        SM_WriteData(TCXXX_ADDRESS,0x0354,0x00000001,4); // DSI_RX_VC_ENABLE
        SM_WriteData(TCXXX_ADDRESS,0x0358,0x000000C8,4); // IND_TO_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0368,0x0000002A,4); // DSI_HSYNC_STOP_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0370,0x000003DA,4);
        SM_WriteData(TCXXX_ADDRESS,0x037C,0x00000081,4); // DSI_TX_MODE
        SM_WriteData(TCXXX_ADDRESS,0x038C,0x00000001,4); // DSI_HSYNC_WIDTH
        SM_WriteData(TCXXX_ADDRESS,0x0390,0x000000DF,4); // DSI_HBPR
        SM_WriteData(TCXXX_ADDRESS,0x03A4,0x00000000,4); // DSI_RX_STATE_INT_MASK
        SM_WriteData(TCXXX_ADDRESS,0x03C0,0x00000015,4); // DSI_LPRX_THRESH_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x0414,0x00000000,4); // APP_SIDE_ERR_INT_MASK
        SM_WriteData(TCXXX_ADDRESS,0x041C,0x00000080,4); // DSI_RX_ERR_INT_MASK
        SM_WriteData(TCXXX_ADDRESS,0x0424,0x00000000,4); // DSI_LPTX_INT_MASK
        SM_WriteData(TCXXX_ADDRESS,0x0454,0x00000008,4); // LPTXTIMECNT
        SM_WriteData(TCXXX_ADDRESS,0x0458,0x00240007,4); // TCLK_HEADERCNT
        SM_WriteData(TCXXX_ADDRESS,0x045C,0x000D0008,4); // TCLK_TRAILCNT
        SM_WriteData(TCXXX_ADDRESS,0x0460,0x000D0008,4); // THS_HEADERCNT
        SM_WriteData(TCXXX_ADDRESS,0x0464,0x00003E80,4); // TWAKEUPCNT
        SM_WriteData(TCXXX_ADDRESS,0x0468,0x0000000D,4); // TCLK_POSTCNT
        SM_WriteData(TCXXX_ADDRESS,0x046C,0x000D0008,4); // THS_TRAILCNT
        SM_WriteData(TCXXX_ADDRESS,0x0470,0x00000020,4); // HSTXVREGCNT
        SM_WriteData(TCXXX_ADDRESS,0x0474,0x0000001F,4); // HSTXVREGEN
        SM_WriteData(TCXXX_ADDRESS,0x0478,0x00080008,4); // BTA_COUNT
        SM_WriteData(TCXXX_ADDRESS,0x047C,0x00000002,4); // DPHY_TX ADJUST
        SM_WriteData(TCXXX_ADDRESS,0x031C,0x00000001,4); // DSITX_START
        // Command Transmission Before Video Start
        SM_WriteData(TCXXX_ADDRESS,0x0500,0x0004,2); // CMD_SEL
        SM_WriteData(TCXXX_ADDRESS,0x0110,0x00000016,4); // MODE_CONFIG
        SM_WriteData(TCXXX_ADDRESS,0x0310,0x00000016,4); // MODE_CONFIG
        // LCD Initialization------------------------------
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0090,2);
        SmDelayms(1);
        //CMD2_Page0 Select
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x8039,2);
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0006,2);
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x55F0,2);
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x52AA,2); 
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0008,2); 
         SmDelayms(1);
         //Scaling Control
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x01C9,2);
        SmDelayms(1);
        //Data Compression Method Selection       
        //    SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
        //    SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0090,2);
        SmDelayms(1);
        //Scaling Up Function Enable
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0158,2);
        SmDelayms(1);
        //Interface Set
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
        //SM_WriteData(TCXXX_ADDRESS, 0x0504,0x01B4,2);//0x01B4 0x10B4,11B4
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x01B4,2);//0x01:command mode,0x10:video mode,0x11:video through ram
        SmDelayms(1);
        //CMD3 Select Enable
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x8039,2);
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0005,2);
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0xAAFF,2);
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0xA555,2); 
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0080,2); 
        SmDelayms(1);
        //Black Display Setting1
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x016F,2);
        SmDelayms(1);
        //Black Display Setting2
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x10F3,2);
        SmDelayms(1);
        //CMD3 Select Disable
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x8039,2);
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0005,2);
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0xAAFF,2);
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0xA555,2); 
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0000,2); 
         SmDelayms(1);
        //CMD2_Page7 Select
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x8039,2);
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0006,2);
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x55F0,2);
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x52AA,2); 
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0708,2); 
         SmDelayms(1);
        //YYG SPR Control
         SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
         SM_WriteData(TCXXX_ADDRESS, 0x0504,0x10DD,2);
         SmDelayms(1);
        //Display Mode Selection
         SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
         SM_WriteData(TCXXX_ADDRESS, 0x0504,0x01EF,2);
         SmDelayms(1);
        //Exit Sleep Mode
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0005,2);
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0011,2);
        SmDelayms(1);
#if 0   
            //CMD2_Page3 Select 20160920 no_change 
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x8039,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0006,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x55F0,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x52AA,2); //闪屏
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0308,2);  
            
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x8039,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0008,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x00B2,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x1200,2); 
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0xA000,2); 
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0203,2);  
#endif
        //Set_Display_On
         SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0005,2);
        SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0029,2);   
        SmDelayms(200);  
        //End LCD Initialization------------------------------
        // Split Control
        SM_WriteData(TCXXX_ADDRESS,0x5000,0x0000,2); // STX0_CTL
        SM_WriteData(TCXXX_ADDRESS,0x500C,0x84E0,2); // STX0_FPX 0x84E0
        SM_WriteData(TCXXX_ADDRESS,0x5080,0x0000,2); // STX1_CTL
        // HDMI PHY
        SM_WriteData(TCXXX_ADDRESS,0x8410,0x03,1); // PHY CTL
        SM_WriteData(TCXXX_ADDRESS,0x8413,0x3F,1); // PHY_ENB
        SM_WriteData(TCXXX_ADDRESS,0x8420,0x07,1); // EQ_BYPS
        SM_WriteData(TCXXX_ADDRESS,0x84F0,0x31,1); // APLL_CTL
        SM_WriteData(TCXXX_ADDRESS,0x84F4,0x01,1); // DDCIO_CTL
        // HDMI Clock
        SM_WriteData(TCXXX_ADDRESS,0x8540,0x12C0,2); // SYS_FREQ0_1
        SM_WriteData(TCXXX_ADDRESS,0x8630,0x00,1); // LOCKDET_FREQ0
        SM_WriteData(TCXXX_ADDRESS,0x8631,0x0753,2); // LOCKDET_REF1_2
        SM_WriteData(TCXXX_ADDRESS,0x8670,0x02,1); // NCO_F0_MOD
        SM_WriteData(TCXXX_ADDRESS,0x8A0C,0x12C0,2); // CSC_SCLK0_1
        // HDMI Interrupt Mask, Clear
        SM_WriteData(TCXXX_ADDRESS,0x850B,0xFF,1); // MISC_INT
        SM_WriteData(TCXXX_ADDRESS,0x851B,0xFD,1); // MISC_INTM
        // Interrupt Control (TOP level)
        SM_WriteData(TCXXX_ADDRESS,0x0014,0x0FBF,2); // IntStatus
        SM_WriteData(TCXXX_ADDRESS,0x0016,0x0DBF,2); // IntMask        

        // EDID
        //8-9: 26d2  ly_20160928
//10-11:03e8
//12-15:000001
//17: 2016   15+5 = 0x1A
        SM_WriteData(TCXXX_ADDRESS,0x85E0,0x01,1); // EDID_MODE
        SM_WriteData(TCXXX_ADDRESS,0x85E3,0x0100,2); // EDID_LEN1_2 
        SM_WriteData(TCXXX_ADDRESS,0x8C00,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C01,0xFF,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C02,0xFF,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C03,0xFF,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C04,0xFF,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C05,0xFF,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C06,0xFF,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C07,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C08,0x26,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C09,0xD2,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C0A,0x03,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C0B,0xE8,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C0C,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C0D,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C0E,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C0F,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C10,0x1C,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C11,0x1A,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C12,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C13,0x03,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C14,0x80,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C15,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C16,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C17,0x78,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C18,0x0A,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C19,0x0D,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C1A,0xC9,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C1B,0xA0,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C1C,0x57,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C1D,0x47,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C1E,0x98,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C1F,0x27,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C20,0x12,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C21,0x48,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C22,0x4C,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C23,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C24,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C25,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C26,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C27,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C28,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C29,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C2A,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C2B,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C2C,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C2D,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C2E,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C2F,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C30,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C31,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C32,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C33,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C34,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C35,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C36,0x6C,1); // EDID_RAM TIM1 1080(54-71)
        SM_WriteData(TCXXX_ADDRESS,0x8C37,0x39,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C38,0x38,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C39,0xBC,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C3A,0x40,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C3B,0x80,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C3C,0x0C,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C3D,0x70,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C3E,0x78,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C3F,0x04,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C40,0x22,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C41,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C42,0x38,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C43,0x80,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C44,0x47,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C45,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C46,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C47,0x18,1); // EDID_RAM TIM1_END		
        SM_WriteData(TCXXX_ADDRESS,0x8C48,0x6C,1); // EDID_RAM TIM2 1440(72-89)
        SM_WriteData(TCXXX_ADDRESS,0x8C49,0x39,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C4A,0x38,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C4B,0xBC,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C4C,0x40,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C4D,0x80,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C4E,0x0C,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C4F,0x70,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C50,0x78,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C51,0x04,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C52,0x22,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C53,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C54,0x38,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C55,0x80,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C56,0x47,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C57,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C58,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C59,0x18,1); // EDID_RAM TIM2 END
        SM_WriteData(TCXXX_ADDRESS,0x8C5A,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C5B,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C5C,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C5D,0xFC,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C5E,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C5F,0x49,1); // EDID_RAM Idealens  
        SM_WriteData(TCXXX_ADDRESS,0x8C60,0x44,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C61,0x45,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C62,0x41,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C63,0x4C,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C64,0x45,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C65,0x4E,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C66,0x53,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C67,0x20,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C68,0x20,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C69,0x20,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C6A,0x20,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C6B,0x0A,1); // EDID_RAM end Idealens       
        SM_WriteData(TCXXX_ADDRESS,0x8C6C,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C6D,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C6E,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C6F,0xFD,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C70,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C71,0x14,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C72,0x78,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C73,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C74,0xFF,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C75,0x1D,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C76,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C77,0x0A,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C78,0x20,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C79,0x20,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C7A,0x20,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C7B,0x20,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C7C,0x20,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C7D,0x20,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C7E,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C7F,0x0D,1); // EDID_RAM SUM
        SM_WriteData(TCXXX_ADDRESS,0x8C80,0x02,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C81,0x03,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C82,0x1A,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C83,0x71,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C84,0x47,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C85,0xC6,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C86,0x46,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C87,0x46,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C88,0x46,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C89,0x46,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C8A,0x46,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C8B,0x46,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C8C,0x23,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C8D,0x09,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C8E,0x07,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C8F,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C90,0x83,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C91,0x01,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C92,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C93,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C94,0x65,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C95,0x03,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C96,0x0C,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C97,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C98,0x10,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C99,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C9A,0x6C,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C9B,0x39,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C9C,0x38,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C9D,0xBC,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C9E,0x40,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8C9F,0x80,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CA0,0x0C,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CA1,0x70,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CA2,0x78,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CA3,0x04,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CA4,0x22,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CA5,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CA6,0x38,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CA7,0x80,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CA8,0x47,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CA9,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CAA,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CAB,0x18,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CAC,0x6C,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CAD,0x39,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CAE,0x38,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CAF,0xBC,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CB0,0x40,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CB1,0x80,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CB2,0x0C,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CB3,0x70,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CB4,0x78,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CB5,0x04,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CB6,0x22,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CB7,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CB8,0x38,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CB9,0x80,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CBA,0x47,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CBB,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CBC,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CBD,0x18,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CBE,0x6C,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CBF,0x39,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CC0,0x38,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CC1,0xBC,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CC2,0x40,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CC3,0x80,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CC4,0x0C,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CC5,0x70,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CC6,0x78,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CC7,0x04,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CC8,0x22,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CC9,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CCA,0x38,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CCB,0x80,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CCC,0x47,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CCD,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CCE,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CCF,0x18,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CD0,0x6C,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CD1,0x39,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CD2,0x38,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CD3,0xBC,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CD4,0x40,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CD5,0x80,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CD6,0x0C,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CD7,0x70,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CD8,0x78,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CD9,0x04,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CDA,0x22,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CDB,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CDC,0x38,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CDD,0x80,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CDE,0x47,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CDF,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CE0,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CE1,0x18,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CE2,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CE3,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CE4,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CE5,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CE6,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CE7,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CE8,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CE9,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CEA,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CEB,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CEC,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CED,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CEE,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CEF,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CF0,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CF1,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CF2,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CF3,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CF4,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CF5,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CF6,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CF7,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CF8,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CF9,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CFA,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CFB,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CFC,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CFD,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CFE,0x00,1); // EDID_RAM
        SM_WriteData(TCXXX_ADDRESS,0x8CFF,0x5B,1); // EDID_RAM
        
        // HDCP Setting
        SM_WriteData(TCXXX_ADDRESS,0x85EC,0x01,1); // 
        SM_WriteData(TCXXX_ADDRESS,0x8560,0x24,1); // HDCP_MODE
        SM_WriteData(TCXXX_ADDRESS,0x8563,0x11,1); // 
        SM_WriteData(TCXXX_ADDRESS,0x8564,0x0F,1); // 
        // Video Color Format Setting
        SM_WriteData(TCXXX_ADDRESS,0x8A02,0x42,1); // VOUT_SYNC0
        // HDMI SYSTEM
        SM_WriteData(TCXXX_ADDRESS,0x8543,0x02,1); // DDC_CTL
        SM_WriteData(TCXXX_ADDRESS,0x8544,0x10,1); // HPD_CTL
        // HDMI Audio Setting
        SM_WriteData(TCXXX_ADDRESS,0x8600,0x00,1); // AUD_Auto_Mute
        SM_WriteData(TCXXX_ADDRESS,0x8602,0xF3,1); // Auto_CMD0
        SM_WriteData(TCXXX_ADDRESS,0x8603,0x02,1); // Auto_CMD1
        SM_WriteData(TCXXX_ADDRESS,0x8604,0x0C,1); // Auto_CMD2
        SM_WriteData(TCXXX_ADDRESS,0x8606,0x05,1); // BUFINIT_START
        SM_WriteData(TCXXX_ADDRESS,0x8607,0x00,1); // FS_MUTE
        SM_WriteData(TCXXX_ADDRESS,0x8652,0x02,1); // SDO_MODE1
        SM_WriteData(TCXXX_ADDRESS,0x8671,0x020C49BA,4); // NCO_48F0A_D
        SM_WriteData(TCXXX_ADDRESS,0x8675,0x01E1B089,4); // NCO_44F0A_D
        SM_WriteData(TCXXX_ADDRESS,0x8680,0x00,1); // AUD_MODE
        // Let HDMI Source start access
        SM_WriteData(TCXXX_ADDRESS,0x854A,0x01,1); // INIT_END
        // HDMI Signal Detection
        // Wait until HDMI sync is established
        // By Interrupt   In Interrupt Service Routine.
         while(1) //  否则在没有HDMi插入的情况下  USB 没有数据出来
          {
                //Wait until HDMI sync is established
                cbit = SM_ReadData(TCXXX_ADDRESS, 0x8520, 1, NULL); // SYS_STATUS Sequence: Check bit7 of 8x8520
                if((cbit>>7)&0x1){
                      HDMI_PLUG = 1;
                                break;
                }
                SmDelayms(100);
          }
         SmDelayms(1000);//add
        //i2c1_uh2d_read8(0x8520); // SYS_STATUS

        // Sequence: Check bit7 of 8x8520
        SM_WriteData(TCXXX_ADDRESS,0x850B,0xFF,1); // MISC_INT
        SM_WriteData(TCXXX_ADDRESS,0x0014,0x0FBF,2); // IntStatus
        // By Polling
        //i2c1_uh2d_read8(0x8520); // SYS_STATUS
         while(1) //  否则在没有HDMi插入的情况下  USB 没有数据出来
          {
                //Wait until HDMI sync is established
                cbit = SM_ReadData(TCXXX_ADDRESS, 0x8520, 1, NULL); // SYS_STATUS Sequence: Check bit7 of 8x8520
                if((cbit>>7)&0x1){
                      HDMI_PLUG = 1;
                                break;
                }
                SmDelayms(100);
          }   
        // Sequence: Check bit7 of 8x8520
        // Start Video TX
        SM_WriteData(TCXXX_ADDRESS,0x0004,0x0C37,2); // ConfCtl0
        SM_WriteData(TCXXX_ADDRESS,0x0006,0x0000,2); // ConfCtl1
        // Command Transmission After Video Start.
        SM_WriteData(TCXXX_ADDRESS,0x0110,0x00000006,4); // MODE_CONFIG
        SM_WriteData(TCXXX_ADDRESS,0x0310,0x00000006,4); // MODE_CONFIG
        SmDelayms(1000);
        /////////////////////////////////////////////////////
//#define _TEST_
#ifdef _TEST_
        SM_WriteData(TCXXX_ADDRESS,0x8540,0xC0,1);
        SM_WriteData(TCXXX_ADDRESS,0x8541,0x12,1);
        SM_WriteData(TCXXX_ADDRESS,0x8A0C,0xC0,1);
        SM_WriteData(TCXXX_ADDRESS,0x8A0D,0x12,1);
        SM_WriteData(TCXXX_ADDRESS,0x8593,0x33,1);
        SmDelayms(100);        
        //PCLK
        ret  = SM_ReadData(TCXXX_ADDRESS, 0x8405, 1, NULL);
        cbit = SM_ReadData(TCXXX_ADDRESS, 0x8406, 1, NULL);        
        cbit = cbit*256 + ret;
        //hp= 0x858F*256+0x858E,vp = 0x8583 *256 +0x8582     
        ret  = SM_ReadData(TCXXX_ADDRESS, 0x858E, 1, NULL);
        cbit = SM_ReadData(TCXXX_ADDRESS, 0x858F, 1, NULL);
        ret  = SM_ReadData(TCXXX_ADDRESS, 0x8580, 1, NULL);
        cbit = SM_ReadData(TCXXX_ADDRESS, 0x8581, 1, NULL);
        
        cbit = cbit*256 + ret;
        ret  = SM_ReadData(TCXXX_ADDRESS, 0x8582, 1, NULL);
        cbit = SM_ReadData(TCXXX_ADDRESS, 0x8583, 1, NULL);//HDP 1920        
        cbit = cbit*256 + ret;
               
        ret  = SM_ReadData(TCXXX_ADDRESS, 0x8590, 1, NULL);
        cbit = SM_ReadData(TCXXX_ADDRESS, 0x8591, 1, NULL);        
        cbit = cbit*256 + ret;
        ret  = SM_ReadData(TCXXX_ADDRESS, 0x8584, 1, NULL);
        cbit = SM_ReadData(TCXXX_ADDRESS, 0x8585, 1, NULL);        
        cbit = cbit*256 + ret;
        cbit  = SM_ReadData(TCXXX_ADDRESS, 0x8586, 1, NULL);
        cbit = SM_ReadData(TCXXX_ADDRESS, 0x8587, 1, NULL);        
        cbit = cbit*256 + ret;
        ret  = SM_ReadData(TCXXX_ADDRESS, 0x858C, 1, NULL);
        cbit = SM_ReadData(TCXXX_ADDRESS, 0x858D, 1, NULL);//VDP 1080        
        cbit = cbit*256 + ret;
        ret  = SM_ReadData(TCXXX_ADDRESS, 0x8526, 1, NULL);
        SmDelayms(1);
#endif
}

//在Screen指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 Screen_ReadByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
        IIC2_Start(); 
	IIC2_Send_Byte(0XA0);	   //发送写命令
	IIC2_Wait_Ack();
	IIC2_Send_Byte(ReadAddr>>8);//发送高地址
	IIC2_Wait_Ack();	
        IIC2_Send_Byte(ReadAddr%256);   //发送低地址
	IIC2_Wait_Ack();	    
	IIC2_Start();  	 	   
	IIC2_Send_Byte(0XA1);           //进入接收模式			   
	IIC2_Wait_Ack();	 
        temp=IIC2_Read_Byte(0);		   
        IIC2_Stop();//产生一个停止条件	    
	return temp;
}
//在Screen指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void Screen_WriteByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
        IIC2_Start();
	IIC2_Send_Byte(0XA0);	    //发送写命令
	IIC2_Wait_Ack();
	IIC2_Send_Byte(WriteAddr>>8);//发送高地址 		 
	IIC2_Wait_Ack();	   
        IIC2_Send_Byte(WriteAddr%256);   //发送低地址
	IIC2_Wait_Ack(); 	 										  		   
	IIC2_Send_Byte(DataToWrite);     //发送字节							   
	IIC2_Wait_Ack();  		    	   
        IIC2_Stop();//产生一个停止条件 
	delay_ms(10);	 
}
//在Screen里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void Screen_WriteBytes(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		Screen_WriteByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//在Screen里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
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

//在Screen里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void Screen_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=Screen_ReadByte(ReadAddr++);	
		NumToRead--;
	}
}  
//在Screen里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void Screen_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		Screen_WriteByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
//////////////////////////////////////////////////
void Vot_WriteByte(u8 WriteAddr,u8 DataToWrite)
{				   	  	    																 
        IIC2_Start();
	IIC2_Send_Byte(Vot_Slave_Address);//发送从机地址(写命令)
	IIC2_Wait_Ack();	   
        IIC2_Send_Byte(WriteAddr);   //发送从机寄存器地址
	IIC2_Wait_Ack(); 	 										  		   
	IIC2_Send_Byte(DataToWrite); //发送字节							   
	IIC2_Wait_Ack();  		    	   
        IIC2_Stop();//产生一个停止条件 
	delay_ms(10);
}
//------------------------------------------------
void iic2_uh2d_write8(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
        IIC2_Start();
	IIC2_Send_Byte(TCXXX_ADDRESS);	    //发送写命令
	IIC2_Wait_Ack();
	IIC2_Send_Byte(WriteAddr>>8);//发送高地址 		 
	IIC2_Wait_Ack();	   
        IIC2_Send_Byte(WriteAddr);   //发送低地址
	IIC2_Wait_Ack(); 	 										  		   
	IIC2_Send_Byte(DataToWrite);     //发送字节							   
	IIC2_Wait_Ack();  		    	   
        IIC2_Stop();//产生一个停止条件 
	delay_ms(10);	 
}

void iic2_uh2d_write16(u16 WriteAddr,u16 DataToWrite)
{				   	  	    																 
        IIC2_Start();
	IIC2_Send_Byte(TCXXX_ADDRESS);	    //发送写命令
	IIC2_Wait_Ack();
	IIC2_Send_Byte(WriteAddr>>8);//发送高地址 		 
	IIC2_Wait_Ack();	   
        IIC2_Send_Byte(WriteAddr);   //发送低地址
	IIC2_Wait_Ack(); 	 										  		   
	IIC2_Send_Byte(DataToWrite);     //发送字节1							   
	IIC2_Wait_Ack(); 
        IIC2_Send_Byte(DataToWrite>>8);  //发送字节2							   
	IIC2_Wait_Ack();  
        IIC2_Stop();//产生一个停止条件 
	delay_ms(10);	 
}

void iic2_uh2d_write32(u16 WriteAddr,u32 DataToWrite)
{				   	  	    																 
        IIC2_Start();
	IIC2_Send_Byte(TCXXX_ADDRESS);	    //发送写命令
	IIC2_Wait_Ack();
	IIC2_Send_Byte(WriteAddr>>8);//发送高地址 		 
	IIC2_Wait_Ack();	   
        IIC2_Send_Byte(WriteAddr);   //发送低地址
	IIC2_Wait_Ack(); 	 										  		   
	IIC2_Send_Byte(DataToWrite);     //发送字节1							   
	IIC2_Wait_Ack(); 
        IIC2_Send_Byte(DataToWrite>>8);  //发送字节2							   
	IIC2_Wait_Ack(); 
        IIC2_Send_Byte(DataToWrite>>16);  //发送字节3							   
	IIC2_Wait_Ack(); 
        IIC2_Send_Byte(DataToWrite>>24);  //发送字节4							   
	IIC2_Wait_Ack(); 
        IIC2_Stop();//产生一个停止条件 
	delay_ms(10);	 
}

u8 iic2_uh2d_read8(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
        IIC2_Start(); 
	IIC2_Send_Byte(TCXXX_ADDRESS);	   //发送写命令
	IIC2_Wait_Ack();
	IIC2_Send_Byte(ReadAddr>>8);    //发送高地址
	IIC2_Wait_Ack();	
        IIC2_Send_Byte(ReadAddr%256);   //发送低地址
	IIC2_Wait_Ack();	    
	IIC2_Start();  	 	   
	IIC2_Send_Byte(TCXXX_ADDRESS | 0x01);           //read (0x7D)			   
	IIC2_Wait_Ack();	 
        temp=IIC2_Read_Byte(0);		   
        IIC2_Stop();	    
	return temp;
}
//uint32 SM_ReadData(uint8 SlaveAddress, uint16 REG_Address, uint8 REG_data_len, uint8 *dest)
u32 SM_ReadData(u8 SlaveAddress, u16 REG_Address, u8 REG_data_len, u8 *dest)
{
        u8 temp=0;		  	    																 
        IIC2_Start(); 
        IIC2_Send_Byte(TCXXX_ADDRESS);	   //write
        IIC2_Wait_Ack();
        IIC2_Send_Byte(REG_Address>>8);    //hign byte
        IIC2_Wait_Ack();	
        IIC2_Send_Byte(REG_Address%256);   //low byte
        IIC2_Wait_Ack();	    
        IIC2_Start();  	 	   
        IIC2_Send_Byte(TCXXX_ADDRESS | 0x01);//read (0x7D)		   
        IIC2_Wait_Ack();	 
        temp=IIC2_Read_Byte(0);		   
        IIC2_Stop();//产生一个停止条件	  
        return temp;
}
u8 SM_WriteData(u8 slaveAddr,u16 address,u32 data,u8 len)
{
        u8 dat[4] = {0},*pDat = 0;
        dat[0] = data&0xFF;
        dat[1] = ((data>>8)&0xFF);
        dat[2] = ((data>>16)&0xFF);
        dat[3] = ((data>>24)&0xFF);
        //send slave address.
        IIC2_Start();
        IIC2_Send_Byte(slaveAddr);
        IIC2_Wait_Ack();
        //send slave reg address.
        IIC2_Send_Byte(address>>8);
        IIC2_Wait_Ack();	   
        IIC2_Send_Byte(address&0xFF);
        IIC2_Wait_Ack(); 
        pDat = dat;
        //send data.
        while(len--){
          IIC2_Send_Byte(*pDat);		
          pDat++;
          IIC2_Wait_Ack();
        }       
        IIC2_Stop();
        //delay_ms(10);	
	return 0;
}
void sharp_lin_1440_2560(void)
{
        u32 cbit = 0;	
        // Recommended i2c speed 100Khz
        // I2C slave address 8'b0001_111x if INT=0 during reset
        // I2C slave address 8'b0011_111x if INT=1 during reset
        // e.g i2c1_uh2d_write32(0x02AC,0x000090BD);
        // i2c write sequence : 0x02 0xAC 0xBD 0x90 0x00 0x00
        // Initialization to receive HDMI signal
       // Software Reset
		SM_WriteData(TCXXX_ADDRESS,0x0004,0x0004,2); // ConfCtl0
            SM_WriteData(TCXXX_ADDRESS,0x0002,0x3F01,2); // SysCtl
            SM_WriteData(TCXXX_ADDRESS,0x0002,0x0000,2); // SysCtl
            SM_WriteData(TCXXX_ADDRESS,0x0006,0x0008,2); // ConfCtl1
            // DSI-TX0 Transition Timing
            SM_WriteData(TCXXX_ADDRESS,0x0108,0x00000001,4); // DSI_TX_CLKEN
            SM_WriteData(TCXXX_ADDRESS,0x010C,0x00000001,4); // DSI_TX_CLKSEL
            SM_WriteData(TCXXX_ADDRESS,0x02A0,0x00000001,4); // MIPI_PLL_CONTROL
            SM_WriteData(TCXXX_ADDRESS,0x02AC,0x0000304F,4); // MIPI_PLL_CNF
            SmDelayms(1);
            SM_WriteData(TCXXX_ADDRESS,0x02A0,0x00000003,4); // MIPI_PLL_CONTROL
            SM_WriteData(TCXXX_ADDRESS,0x0118,0x00000014,4); // LANE_ENABLE
            SM_WriteData(TCXXX_ADDRESS,0x0120,0x00001770,4); // LINE_INIT_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0124,0x00000000,4); // HSTX_TO_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0128,0x00000101,4); // FUNC_ENABLE
            SM_WriteData(TCXXX_ADDRESS,0x0130,0x00010000,4); // DSI_TATO_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0134,0x00005000,4); // DSI_PRESP_BTA_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0138,0x00010000,4); // DSI_PRESP_LPR_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x013C,0x00010000,4); // DSI_PRESP_LPW_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0140,0x00010000,4); // DSI_PRESP_HSR_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0144,0x00010000,4); // DSI_PRESP_HSW_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0148,0x00001000,4); // DSI_PR_TO_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x014C,0x00010000,4); // DSI_LRX-H_TO_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0150,0x00000141,4); // FUNC_MODE
            SM_WriteData(TCXXX_ADDRESS,0x0154,0x00000001,4); // DSI_RX_VC_ENABLE
            SM_WriteData(TCXXX_ADDRESS,0x0158,0x000000C8,4); // IND_TO_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0168,0x0000002A,4); // DSI_HSYNC_STOP_COUNT 
            SM_WriteData(TCXXX_ADDRESS,0x0170,0x000002EB,4);
            SM_WriteData(TCXXX_ADDRESS,0x017C,0x00000081,4); // DSI_TX_MODE
            SM_WriteData(TCXXX_ADDRESS,0x018C,0x00000001,4); // DSI_HSYNC_WIDTH
            SM_WriteData(TCXXX_ADDRESS,0x0190,0x000000DE,4); // DSI_HBPR
            SM_WriteData(TCXXX_ADDRESS,0x01A4,0x00000000,4); // DSI_RX_STATE_INT_MASK
            SM_WriteData(TCXXX_ADDRESS,0x01C0,0x00000015,4); // DSI_LPRX_THRESH_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0214,0x00000000,4); // APP_SIDE_ERR_INT_MASK
            SM_WriteData(TCXXX_ADDRESS,0x021C,0x00000080,4); // DSI_RX_ERR_INT_MASK
            SM_WriteData(TCXXX_ADDRESS,0x0224,0x00000000,4); // DSI_LPTX_INT_MASK
            SM_WriteData(TCXXX_ADDRESS,0x0254,0x00000008,4); // LPTXTIMECNT
            SM_WriteData(TCXXX_ADDRESS,0x0258,0x00240007,4); // TCLK_HEADERCNT
            SM_WriteData(TCXXX_ADDRESS,0x025C,0x000D0008,4); // TCLK_TRAILCNT
            SM_WriteData(TCXXX_ADDRESS,0x0260,0x000D0008,4); // THS_HEADERCNT
            SM_WriteData(TCXXX_ADDRESS,0x0264,0x00003E80,4); // TWAKEUPCNT
            SM_WriteData(TCXXX_ADDRESS,0x0268,0x0000000D,4); // TCLK_POSTCNT
            SM_WriteData(TCXXX_ADDRESS,0x026C,0x000D0008,4); // THS_TRAILCNT
            SM_WriteData(TCXXX_ADDRESS,0x0270,0x00000020,4); // HSTXVREGCNT
            SM_WriteData(TCXXX_ADDRESS,0x0274,0x0000001F,4); // HSTXVREGEN
            SM_WriteData(TCXXX_ADDRESS,0x0278,0x00080008,4); // BTA_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x027C,0x00000002,4); // DPHY_TX ADJUST
            SM_WriteData(TCXXX_ADDRESS,0x011C,0x00000001,4); // DSITX_START
            // DSI-TX1 Transition Timing
            SM_WriteData(TCXXX_ADDRESS,0x0308,0x00000001,4); // DSI_TX_CLKEN
            SM_WriteData(TCXXX_ADDRESS,0x030C,0x00000001,4); // DSI_TX_CLKSEL
            SM_WriteData(TCXXX_ADDRESS,0x04A0,0x00000001,4); // MIPI_PLL_CONTROL
            SM_WriteData(TCXXX_ADDRESS,0x04AC,0x0000304F,4); // MIPI_PLL_CNF
            SmDelayms(1);
            SM_WriteData(TCXXX_ADDRESS,0x04A0,0x00000003,4); // MIPI_PLL_CONTROL
            SM_WriteData(TCXXX_ADDRESS,0x0318,0x00000014,4); // LANE_ENABLE
            SM_WriteData(TCXXX_ADDRESS,0x0320,0x00001770,4); // LINE_INIT_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0324,0x00000000,4); // HSTX_TO_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0328,0x00000101,4); // FUNC_ENABLE
            SM_WriteData(TCXXX_ADDRESS,0x0330,0x00010000,4); // DSI_TATO_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0334,0x00005000,4); // DSI_PRESP_BTA_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0338,0x00010000,4); // DSI_PRESP_LPR_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x033C,0x00010000,4); // DSI_PRESP_LPW_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0340,0x00010000,4); // DSI_PRESP_HSR_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0344,0x00010000,4); // DSI_PRESP_HSW_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0348,0x00001000,4); // DSI_PR_TO_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x034C,0x00010000,4); // DSI_LRX-H_TO_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0350,0x00000141,4); // FUNC_MODE
            SM_WriteData(TCXXX_ADDRESS,0x0354,0x00000001,4); // DSI_RX_VC_ENABLE
            SM_WriteData(TCXXX_ADDRESS,0x0358,0x000000C8,4); // IND_TO_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0368,0x0000002A,4); // DSI_HSYNC_STOP_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0370,0x000002EB,4);
            SM_WriteData(TCXXX_ADDRESS,0x037C,0x00000081,4); // DSI_TX_MODE
            SM_WriteData(TCXXX_ADDRESS,0x038C,0x00000001,4); // DSI_HSYNC_WIDTH
            SM_WriteData(TCXXX_ADDRESS,0x0390,0x000000DE,4); // DSI_HBPR
            SM_WriteData(TCXXX_ADDRESS,0x03A4,0x00000000,4); // DSI_RX_STATE_INT_MASK
            SM_WriteData(TCXXX_ADDRESS,0x03C0,0x00000015,4); // DSI_LPRX_THRESH_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x0414,0x00000000,4); // APP_SIDE_ERR_INT_MASK
            SM_WriteData(TCXXX_ADDRESS,0x041C,0x00000080,4); // DSI_RX_ERR_INT_MASK
            SM_WriteData(TCXXX_ADDRESS,0x0424,0x00000000,4); // DSI_LPTX_INT_MASK
            SM_WriteData(TCXXX_ADDRESS,0x0454,0x00000008,4); // LPTXTIMECNT
            SM_WriteData(TCXXX_ADDRESS,0x0458,0x00240007,4); // TCLK_HEADERCNT
            SM_WriteData(TCXXX_ADDRESS,0x045C,0x000D0008,4); // TCLK_TRAILCNT
            SM_WriteData(TCXXX_ADDRESS,0x0460,0x000D0008,4); // THS_HEADERCNT
            SM_WriteData(TCXXX_ADDRESS,0x0464,0x00003E80,4); // TWAKEUPCNT
            SM_WriteData(TCXXX_ADDRESS,0x0468,0x0000000D,4); // TCLK_POSTCNT
            SM_WriteData(TCXXX_ADDRESS,0x046C,0x000D0008,4); // THS_TRAILCNT
            SM_WriteData(TCXXX_ADDRESS,0x0470,0x00000020,4); // HSTXVREGCNT
            SM_WriteData(TCXXX_ADDRESS,0x0474,0x0000001F,4); // HSTXVREGEN
            SM_WriteData(TCXXX_ADDRESS,0x0478,0x00080008,4); // BTA_COUNT
            SM_WriteData(TCXXX_ADDRESS,0x047C,0x00000002,4); // DPHY_TX ADJUST
            SM_WriteData(TCXXX_ADDRESS,0x031C,0x00000001,4); // DSITX_START
            // Command Transmission Before Video Start
            SM_WriteData(TCXXX_ADDRESS,0x0500,0x0004,2); // CMD_SEL
            SM_WriteData(TCXXX_ADDRESS,0x0110,0x00000016,4); // MODE_CONFIG
            SM_WriteData(TCXXX_ADDRESS,0x0310,0x00000016,4); // MODE_CONFIG
            // LCD Initialization------------------------------
               //     SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
       //     SM_WriteData(TCXXX_ADDRESS, 0x0504,0x10C9,2);        
        //    SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
        //    SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0190,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0190,2);//1080:0x0090->0x0190 WQHD
            SmDelayms(1);       
            //CMD2_Page0 Select
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x8039,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0006,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x55F0,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x52AA,2); 
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0008,2); 
             SmDelayms(1);
             //Scaling Control
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x10C9,2);//1080:0x01C9->0x10C9 WQHD
            SmDelayms(1);
            //Data Compression Method Selection       
            //    SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
            //    SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0090,2);
            SmDelayms(1);
            //Scaling Up Function Enable
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0158,2);
            SmDelayms(1);
            //Interface Set
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x01B4,2);//0x01:command mode,0x10:video mode,0x11:video through ram
            SmDelayms(1);
            //CMD3 Select Enable
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x8039,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0005,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0xAAFF,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0xA555,2); 
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0080,2); 
            SmDelayms(1);
            //Black Display Setting1
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x016F,2);
            SmDelayms(1);
            //Black Display Setting2
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x10F3,2);
            SmDelayms(1);
            //CMD3 Select Disable
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x8039,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0005,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0xAAFF,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0xA555,2); 
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0000,2); 
             SmDelayms(1);
            //CMD2_Page7 Select
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x8039,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0006,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x55F0,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x52AA,2); 
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0708,2); 
             SmDelayms(1);
            //YYG SPR Control
             SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
             SM_WriteData(TCXXX_ADDRESS, 0x0504,0x10DD,2);
             SmDelayms(1);             
           //  SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
           // SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0035,2);//add 
            //Display Mode Selection
             SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0015,2);
             SM_WriteData(TCXXX_ADDRESS, 0x0504,0x01EF,2);
             SmDelayms(1);
            //Exit Sleep Mode
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0005,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0011,2);
            SmDelayms(1);
    #if 1   
            //CMD2_Page3 Select 20160920 no_change 
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x8039,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0006,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x55F0,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x52AA,2); //闪屏
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0308,2);  
            
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x8039,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0008,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x00B2,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x1200,2); 
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0xA000,2); 
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0203,2);  
    #endif
            //Set_Display_On
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0005,2);
            SM_WriteData(TCXXX_ADDRESS, 0x0504,0x0029,2);   
            SmDelayms(200);  
            //End LCD Initialization------------------------------
            // Split Control
            SM_WriteData(TCXXX_ADDRESS,0x5000,0x0000,2); // STX0_CTL
            SM_WriteData(TCXXX_ADDRESS,0x500C,0x84E0,2); // STX0_FPX 0x84E0
            SM_WriteData(TCXXX_ADDRESS,0x5080,0x0000,2); // STX1_CTL
            // HDMI PHY
            SM_WriteData(TCXXX_ADDRESS,0x8410,0x03,1); // PHY CTL
            SM_WriteData(TCXXX_ADDRESS,0x8413,0x3F,1); // PHY_ENB
            SM_WriteData(TCXXX_ADDRESS,0x8420,0x07,1); // EQ_BYPS
            SM_WriteData(TCXXX_ADDRESS,0x84F0,0x31,1); // APLL_CTL
            SM_WriteData(TCXXX_ADDRESS,0x84F4,0x01,1); // DDCIO_CTL
            // HDMI Clock
            SM_WriteData(TCXXX_ADDRESS,0x8540,0x12C0,2); // SYS_FREQ0_1
            SM_WriteData(TCXXX_ADDRESS,0x8630,0x00,1); // LOCKDET_FREQ0
            SM_WriteData(TCXXX_ADDRESS,0x8631,0x0753,2); // LOCKDET_REF1_2
            SM_WriteData(TCXXX_ADDRESS,0x8670,0x02,1); // NCO_F0_MOD
            SM_WriteData(TCXXX_ADDRESS,0x8A0C,0x12C0,2); // CSC_SCLK0_1
            // HDMI Interrupt Mask, Clear
            SM_WriteData(TCXXX_ADDRESS,0x850B,0xFF,1); // MISC_INT
            SM_WriteData(TCXXX_ADDRESS,0x851B,0xFD,1); // MISC_INTM
            // Interrupt Control (TOP level)
            SM_WriteData(TCXXX_ADDRESS,0x0014,0x0FBF,2); // IntStatus
            SM_WriteData(TCXXX_ADDRESS,0x0016,0x0DBF,2); // IntMask    
            // EDID 
//8-9: 26d2  ly_20160928
//10-11:03e8
//12-15:000001
//17: 2016   15+5 = 0x1A
            SM_WriteData(TCXXX_ADDRESS,0x85E0,0x01,1); // EDID_MODE
            SM_WriteData(TCXXX_ADDRESS,0x85E3,0x0100,2); // EDID_LEN1_2 
            SM_WriteData(TCXXX_ADDRESS,0x8C00,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C01,0xFF,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C02,0xFF,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C03,0xFF,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C04,0xFF,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C05,0xFF,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C06,0xFF,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C07,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C08,0x26,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C09,0xD2,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C0A,0x03,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C0B,0xE8,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C0C,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C0D,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C0E,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C0F,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C10,0x1C,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C11,0x1A,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C12,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C13,0x03,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C14,0x80,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C15,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C16,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C17,0x78,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C18,0x0A,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C19,0x0D,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C1A,0xC9,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C1B,0xA0,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C1C,0x57,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C1D,0x47,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C1E,0x98,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C1F,0x27,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C20,0x12,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C21,0x48,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C22,0x4C,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C23,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C24,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C25,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C26,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C27,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C28,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C29,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C2A,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C2B,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C2C,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C2D,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C2E,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C2F,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C30,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C31,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C32,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C33,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C34,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C35,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C36,0x90,1); // EDID_RAM TIM1 1440(54-71)
            SM_WriteData(TCXXX_ADDRESS,0x8C37,0x65,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C38,0xA0,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C39,0xED,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C3A,0x50,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C3B,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C3C,0x18,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C3D,0xA0,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C3E,0x75,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C3F,0x28,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C40,0x88,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C41,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C42,0xA0,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C43,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C44,0x5A,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C45,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C46,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C47,0x18,1); // EDID_RAM TIM1_END
            SM_WriteData(TCXXX_ADDRESS,0x8C48,0x6C,1); // EDID_RAM TIM2 1080(72-89)
            SM_WriteData(TCXXX_ADDRESS,0x8C49,0x39,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C4A,0x38,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C4B,0xBC,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C4C,0x40,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C4D,0x80,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C4E,0x0C,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C4F,0x70,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C50,0x78,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C51,0x04,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C52,0x22,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C53,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C54,0x38,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C55,0x80,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C56,0x47,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C57,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C58,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C59,0x18,1); // EDID_RAM TIM2 END
            SM_WriteData(TCXXX_ADDRESS,0x8C5A,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C5B,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C5C,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C5D,0xFC,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C5E,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C5F,0x49,1); // EDID_RAM Idealens  
            SM_WriteData(TCXXX_ADDRESS,0x8C60,0x44,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C61,0x45,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C62,0x41,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C63,0x4C,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C64,0x45,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C65,0x4E,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C66,0x53,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C67,0x20,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C68,0x20,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C69,0x20,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C6A,0x20,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C6B,0x0A,1); // EDID_RAM end Idealens
            SM_WriteData(TCXXX_ADDRESS,0x8C6C,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C6D,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C6E,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C6F,0xFD,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C70,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C71,0x14,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C72,0x78,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C73,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C74,0xFF,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C75,0x1D,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C76,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C77,0x0A,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C78,0x20,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C79,0x20,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C7A,0x20,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C7B,0x20,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C7C,0x20,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C7D,0x20,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C7E,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C7F,0xD6,1); // EDID_RAM SUM
            SM_WriteData(TCXXX_ADDRESS,0x8C80,0x02,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C81,0x03,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C82,0x1A,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C83,0x71,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C84,0x47,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C85,0xC6,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C86,0x46,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C87,0x46,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C88,0x46,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C89,0x46,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C8A,0x46,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C8B,0x46,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C8C,0x23,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C8D,0x09,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C8E,0x07,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C8F,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C90,0x83,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C91,0x01,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C92,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C93,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C94,0x65,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C95,0x03,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C96,0x0C,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C97,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C98,0x10,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C99,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C9A,0x90,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C9B,0x65,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C9C,0xA0,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C9D,0xED,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C9E,0x50,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8C9F,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CA0,0x18,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CA1,0xA0,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CA2,0x75,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CA3,0x28,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CA4,0x88,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CA5,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CA6,0xA0,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CA7,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CA8,0x5A,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CA9,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CAA,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CAB,0x18,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CAC,0x90,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CAD,0x65,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CAE,0xA0,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CAF,0xED,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CB0,0x50,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CB1,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CB2,0x18,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CB3,0xA0,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CB4,0x75,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CB5,0x28,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CB6,0x88,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CB7,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CB8,0xA0,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CB9,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CBA,0x5A,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CBB,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CBC,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CBD,0x18,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CBE,0x90,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CBF,0x65,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CC0,0xA0,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CC1,0xED,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CC2,0x50,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CC3,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CC4,0x18,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CC5,0xA0,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CC6,0x75,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CC7,0x28,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CC8,0x88,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CC9,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CCA,0xA0,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CCB,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CCC,0x5A,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CCD,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CCE,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CCF,0x18,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CD0,0x90,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CD1,0x65,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CD2,0xA0,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CD3,0xED,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CD4,0x50,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CD5,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CD6,0x18,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CD7,0xA0,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CD8,0x75,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CD9,0x28,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CDA,0x88,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CDB,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CDC,0xA0,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CDD,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CDE,0x5A,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CDF,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CE0,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CE1,0x18,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CE2,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CE3,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CE4,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CE5,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CE6,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CE7,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CE8,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CE9,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CEA,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CEB,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CEC,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CED,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CEE,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CEF,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CF0,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CF1,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CF2,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CF3,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CF4,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CF5,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CF6,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CF7,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CF8,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CF9,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CFA,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CFB,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CFC,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CFD,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CFE,0x00,1); // EDID_RAM
            SM_WriteData(TCXXX_ADDRESS,0x8CFF,0x7F,1); // EDID_RAM
            
            // HDCP Setting
            SM_WriteData(TCXXX_ADDRESS,0x85EC,0x01,1); // 
            SM_WriteData(TCXXX_ADDRESS,0x8560,0x24,1); // HDCP_MODE
            SM_WriteData(TCXXX_ADDRESS,0x8563,0x11,1); // 
            SM_WriteData(TCXXX_ADDRESS,0x8564,0x0F,1); // 
            // Video Color Format Setting
            SM_WriteData(TCXXX_ADDRESS,0x8A02,0x42,1); // VOUT_SYNC0
            // HDMI SYSTEM
            SM_WriteData(TCXXX_ADDRESS,0x8543,0x02,1); // DDC_CTL
            SM_WriteData(TCXXX_ADDRESS,0x8544,0x10,1); // HPD_CTL
            // HDMI Audio Setting
            SM_WriteData(TCXXX_ADDRESS,0x8600,0x00,1); // AUD_Auto_Mute
            SM_WriteData(TCXXX_ADDRESS,0x8602,0xF3,1); // Auto_CMD0
            SM_WriteData(TCXXX_ADDRESS,0x8603,0x02,1); // Auto_CMD1
            SM_WriteData(TCXXX_ADDRESS,0x8604,0x0C,1); // Auto_CMD2
            SM_WriteData(TCXXX_ADDRESS,0x8606,0x05,1); // BUFINIT_START
            SM_WriteData(TCXXX_ADDRESS,0x8607,0x00,1); // FS_MUTE
            SM_WriteData(TCXXX_ADDRESS,0x8652,0x02,1); // SDO_MODE1
            SM_WriteData(TCXXX_ADDRESS,0x8671,0x020C49BA,4); // NCO_48F0A_D
            SM_WriteData(TCXXX_ADDRESS,0x8675,0x01E1B089,4); // NCO_44F0A_D
            SM_WriteData(TCXXX_ADDRESS,0x8680,0x00,1); // AUD_MODE
            // Let HDMI Source start access
            SM_WriteData(TCXXX_ADDRESS,0x854A,0x01,1); // INIT_END
            // HDMI Signal Detection
            // Wait until HDMI sync is established
            // By Interrupt   In Interrupt Service Routine.
             while(1) //  否则在没有HDMi插入的情况下  USB 没有数据出来
              {
                    //Wait until HDMI sync is established
                    cbit = SM_ReadData(TCXXX_ADDRESS, 0x8520, 1, NULL); // SYS_STATUS Sequence: Check bit7 of 8x8520
                    if((cbit>>7)&0x1){
                          HDMI_PLUG = 1;
                                    break;
                    }
                    SmDelayms(100);
              }
             SmDelayms(1000);//add
            //i2c1_uh2d_read8(0x8520); // SYS_STATUS
    
            // Sequence: Check bit7 of 8x8520
            SM_WriteData(TCXXX_ADDRESS,0x850B,0xFF,1); // MISC_INT
            SM_WriteData(TCXXX_ADDRESS,0x0014,0x0FBF,2); // IntStatus
            // By Polling
            //i2c1_uh2d_read8(0x8520); // SYS_STATUS
             while(1) //  否则在没有HDMi插入的情况下  USB 没有数据出来
              {
                    //Wait until HDMI sync is established
                    cbit = SM_ReadData(TCXXX_ADDRESS, 0x8520, 1, NULL); // SYS_STATUS Sequence: Check bit7 of 8x8520
                    if((cbit>>7)&0x1){
                          HDMI_PLUG = 1;
                                    break;
                    }
                    SmDelayms(100);
              }   
            // Sequence: Check bit7 of 8x8520
            // Start Video TX
            SM_WriteData(TCXXX_ADDRESS,0x0004,0x0C37,2); // ConfCtl0
            SM_WriteData(TCXXX_ADDRESS,0x0006,0x0000,2); // ConfCtl1
            // Command Transmission After Video Start.
            SM_WriteData(TCXXX_ADDRESS,0x0110,0x00000006,4); // MODE_CONFIG
            SM_WriteData(TCXXX_ADDRESS,0x0310,0x00000006,4); // MODE_CONFIG
            SmDelayms(1000);
            /////////////////////////////////////////////////////
    //#define _TEST_
    #ifdef _TEST_
            SM_WriteData(TCXXX_ADDRESS,0x8540,0xC0,1);
            SM_WriteData(TCXXX_ADDRESS,0x8541,0x12,1);
            SM_WriteData(TCXXX_ADDRESS,0x8A0C,0xC0,1);
            SM_WriteData(TCXXX_ADDRESS,0x8A0D,0x12,1);
            SM_WriteData(TCXXX_ADDRESS,0x8593,0x33,1);
            SmDelayms(100);        
            //PCLK
            ret  = SM_ReadData(TCXXX_ADDRESS, 0x8405, 1, NULL);
            cbit = SM_ReadData(TCXXX_ADDRESS, 0x8406, 1, NULL);        
            cbit = cbit*256 + ret;
            //hp= 0x858F*256+0x858E,vp = 0x8583 *256 +0x8582     
            ret  = SM_ReadData(TCXXX_ADDRESS, 0x858E, 1, NULL);
            cbit = SM_ReadData(TCXXX_ADDRESS, 0x858F, 1, NULL);
            ret  = SM_ReadData(TCXXX_ADDRESS, 0x8580, 1, NULL);
            cbit = SM_ReadData(TCXXX_ADDRESS, 0x8581, 1, NULL);
            
            cbit = cbit*256 + ret;
            ret  = SM_ReadData(TCXXX_ADDRESS, 0x8582, 1, NULL);
            cbit = SM_ReadData(TCXXX_ADDRESS, 0x8583, 1, NULL);//HDP 1920        
            cbit = cbit*256 + ret;
                   
            ret  = SM_ReadData(TCXXX_ADDRESS, 0x8590, 1, NULL);
            cbit = SM_ReadData(TCXXX_ADDRESS, 0x8591, 1, NULL);        
            cbit = cbit*256 + ret;
            ret  = SM_ReadData(TCXXX_ADDRESS, 0x8584, 1, NULL);
            cbit = SM_ReadData(TCXXX_ADDRESS, 0x8585, 1, NULL);        
            cbit = cbit*256 + ret;
            cbit  = SM_ReadData(TCXXX_ADDRESS, 0x8586, 1, NULL);
            cbit = SM_ReadData(TCXXX_ADDRESS, 0x8587, 1, NULL);        
            cbit = cbit*256 + ret;
            ret  = SM_ReadData(TCXXX_ADDRESS, 0x858C, 1, NULL);
            cbit = SM_ReadData(TCXXX_ADDRESS, 0x858D, 1, NULL);//VDP 1080        
            cbit = cbit*256 + ret;
            ret  = SM_ReadData(TCXXX_ADDRESS, 0x8526, 1, NULL);
            SmDelayms(1);
    #endif
}
/*************************END OF FILE*************************************/

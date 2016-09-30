#ifndef __MAIN_H
#define __MAIN_H

//外部变量声明  	
int   temp1[3], temp2[3];											  	  
extern u8    USB_ReceiveFlg;
extern u8    Tx_Buffer[nReportCnt];
extern u8 InReport[];
extern u8 HDMI_PLUG;

extern void USB_SendString(u8 *str);
extern void PANEL_GPIO_RESET_PIN(u8 flag);
extern void  sharp_lin_14402560(void);

void tc358870_hotplug(void);  
void Timer2Configuration(void);
#endif





































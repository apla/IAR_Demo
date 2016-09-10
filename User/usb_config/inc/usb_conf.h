/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_conf.h
* Author             : MCD Application Team
* Version            : V2.2.1
* Date               : 09/22/2008
* Description        : Custom HID demo configuration file
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#include <stdint.h>

#define __IO  volatile 
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
//typedef __IO uint32_t  vu32;
#define vu32 volatile unsigned int

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CONF_H
#define __USB_CONF_H
#ifndef FunctionalState
//typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#endif
typedef enum {DISABLE1 = 0, ENABLE1 = !DISABLE1} FunctionalState1;
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* External variables --------------------------------------------------------*/
/*-------------------------------------------------------------*/
/* EP_NUM */
/* defines how many endpoints are used by the device */
/*-------------------------------------------------------------*/

#if 1
#define EP_NUM     (3)

/*-------------------------------------------------------------*/
/* --------------   Buffer Description Table  -----------------*/
/*-------------------------------------------------------------*/
/* buffer table base address */
/* buffer table base address */
#define BTABLE_ADDRESS      (0x00)

/* EP0  */
/* rx/tx buffer base address */
#define ENDP0_RXADDR        (0x18)
#define ENDP0_TXADDR        (0x58)

/* EP1  */
/* tx buffer base address */
//��ַΪ32λ����,λ4�ı��������ܳ��� bMaxPacketSize

#define ENDP1_RXADDR        (0x98)
#define ENDP1_TXADDR        (0xD8)
//EP2
//#define ENDP2_RXADDR        (0x118)
#define ENDP2_TXADDR        (0x118)

#else
#define EP_NUM     (4)
#define BTABLE_ADDRESS      (0x00)
/* config the endpoint address for 4 EPs */
/* EP0  */
/* rx/tx buffer base address */
#define ENDP0_RXADDR        (0x58)
#define ENDP0_TXADDR        (ENDP0_RXADDR+0x40)

/* EP1  */
/* rx/tx buffer base address */
#define ENDP1_TXADDR        (ENDP0_TXADDR+0x40)
//#define ENDP1_RXADDR        (ENDP1_TXADDR+0x10)

/*EP2 */
/* rx/tx buffer base address */
#define ENDP2_TXADDR        (ENDP1_TXADDR+0x40)
//#define ENDP2_RXADDR        (ENDP2_TXADDR+0x10)

/*EP3 */
/* rx/tx buffer base address */
#define ENDP3_TXADDR        (ENDP2_TXADDR+0x40)
#endif
/*-------------------------------------------------------------*/
/* -------------------   ISTR events  -------------------------*/
/*-------------------------------------------------------------*/
/* IMR_MSK */
/* mask defining which events has to be handled */
/* by the device application software */
#define IMR_MSK (CNTR_CTRM  | CNTR_WKUPM | CNTR_SUSPM | CNTR_ERRM  | CNTR_SOFM \
                 | CNTR_ESOFM | CNTR_RESETM )

/* CTR service routines */
/* associated to defined endpoints */
#define  EP1_IN_Callback   NOP_Process
//#define  EP2_IN_Callback   NOP_Process
#define  EP3_IN_Callback   NOP_Process
#define  EP4_IN_Callback   NOP_Process
#define  EP5_IN_Callback   NOP_Process
#define  EP6_IN_Callback   NOP_Process
#define  EP7_IN_Callback   NOP_Process

//#define  EP1_OUT_Callback   NOP_Process
#define  EP2_OUT_Callback   NOP_Process
#define  EP3_OUT_Callback   NOP_Process
#define  EP4_OUT_Callback   NOP_Process
#define  EP5_OUT_Callback   NOP_Process
#define  EP6_OUT_Callback   NOP_Process
#define  EP7_OUT_Callback   NOP_Process

#endif /*__USB_CONF_H*/

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/


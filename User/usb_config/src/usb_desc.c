/**
  ******************************************************************************
  * @file    usb_desc.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Descriptors for Custom HID Demo
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
const uint8_t CustomHID_DeviceDescriptor[CUSTOMHID_SIZ_DEVICE_DESC] =
  {
    0x12,                       /*bLength = 18byte(fixed)*/
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType (fixed:0x01)*/
    0x00,                       /*bcdUSB (USB2.0)*/
    0x02,
    0x00,                       /*bDeviceClass 0(所有接口在配置描述符里，并且所有接口是独立的。)*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    0x40,                       /*bMaxPacketSize0 =64 端点０最大分组大小（只有8,16,32,64有效）*/
    0x83,                       /*idVendor = 0x0483*/
    0x04,
    0x54,                       /*idProduct = 0x5754*/
    0x57,
    0x00,                       /*bcdDevice rel. 2.00 设备出产编码．由厂家自行设置*/
    0x02,
    1,                          /*Index of string descriptor describing
                                              manufacturer --CustomHID_StringVendor*/
    2,                          /*Index of string descriptor describing
                                             product  --CustomHID_StringProduct*/
    3,                          /*Index of string descriptor describing the
                                             device serial number --CustomHID_StringSerial*/
    0x01                        /*bNumConfigurations 配置字符串的个数*/
  }
  ; /* CustomHID_DeviceDescriptor */


/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t CustomHID_ConfigDescriptor[CUSTOMHID_SIZ_CONFIG_DESC] =
  {
   0x09, /* bLength: Configuation Descriptor size 配置描述符*/
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    CUSTOMHID_SIZ_CONFIG_DESC,
    /* wTotalLength: Bytes returned */
    0x00,
    0x01,         /* bNumInterfaces: 1 interface */
    0x01,         /* bConfigurationValue: Configuration value */
    0x00,         /* iConfiguration: Index of string descriptor describing
                                 the configuration*/
    0x80,         /* bmAttributes: Bus powered 0xC0?*/
                  /*Bus powered: 7th bit, Self Powered: 6th bit, Remote wakeup: 5th bit, reserved: 4..0 bits */
    0x32,         /* 0x32 *2 = MaxPower 100 mA: this current is used for detecting Vbus */
//    0x96,         /* MaxPower 300 mA: this current is used for detecting Vbus */
    /************** Descriptor of Custom HID interface 接口描述符****************/
    /* 09 */  
    0x09,         /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,/* bDescriptorType: Interface descriptor type */
    0x00,         /* bInterfaceNumber: No of Interface该接口的编号 */
    0x00,         /* bAlternateSetting: Alternate setting */
    0x02,         /* bNumEndpoints 使用的端点数目．端点０除外*/
    0x03,         /* bInterfaceClass: HID */
    0x00,         /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00,         /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,            /* iInterface: Index of string descriptor */
    /******************** Descriptor of Custom HID HID (HID描述符)********************/
    /* 18 */
    0x09,         /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE, /* bDescriptorType: HID */
    0x10,         /* bcdHID: HID Class Spec release number HID版本号*/
    0x01,         
    0x00,         /* bCountryCode: Hardware target country 国家地理代码*/
    0x01,         /* bNumDescriptors: Number of HID class descriptors to follow 支持的其他描述符的数量*/
    0x22,         /* bDescriptorType 类别描述符类型*/
    CUSTOMHID_SIZ_REPORT_DESC,/* wItemLength: Total length of Report descriptor */
    0x00,   /*next: wDescriptionLength:optional*/
    /******************** Descriptor of Custom HID endpoints ******************/
    /* 27 */
    0x07,          /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: */

    0x82,          /* bEndpointAddress: Endpoint Address (IN) */               
                   // bit 3...0 : The endpoint number=2
                   // bit 6...4 : reserved
                   // bit 7     : 0(OUT), 1(IN)
    0x03,          /* bmAttributes: Interrupt endpoint BIt1-0：00控制，01同步，02批量，03中断．*/
    0x40,          /* wMaxPacketSize: 64 Bytes max 本端点接收或发送的最大信息包大小*/
    0x00,     
    0x20,          /* bInterval: Polling Interval (32 ms) 轮训数据传送端点的时间间隔．对于批量传送和控制传送的端点忽略．对于同步传送的端点，必须为１，对于中断传送的端点，范围为１－２５５．*/
    /* 34 */
    	
    0x07,	/* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType: */
			/*	Endpoint descriptor type */
    0x01,	/* bEndpointAddress: number=1*/
		/* Endpoint Address (OUT) */
    0x03,	/* bmAttributes: Interrupt endpoint */
    0x40,	/* wMaxPacketSize: 64 Bytes max  */
    0x00,
    0x10,	/* bInterval: Polling Interval (16 ms) */
    /* 41 */
  }
  ; /* CustomHID_ConfigDescriptor */
const uint8_t CustomHID_ReportDescriptor[CUSTOMHID_SIZ_REPORT_DESC] =
  {                    
    0x05, 0x8c, /* USAGE_PAGE (ST Page) */
        0x09, 0x01, /* USAGE (Demo Kit) */
        0xa1, 0x01, /* COLLECTION (Application) */
         
        // The Input report 
        0x09,0x03, // USAGE ID - Vendor defined 
        0x15,0x00, // LOGICAL_MINIMUM (0) 
        0x26,0x00, 0xFF, // LOGICAL_MAXIMUM (255) 
        0x75,0x08, // REPORT_SIZE (8bit) 
        0x95,0x40, // REPORT_COUNT (64Byte) 
        0x81,0x02, // INPUT (Data,Var,Abs) 
 
        // The Output report 
        0x09,0x04, // USAGE ID - Vendor defined 
        0x15,0x00, // LOGICAL_MINIMUM (0) 
        0x26,0x00,0xFF, // LOGICAL_MAXIMUM (255) 
        0x75,0x08, // REPORT_SIZE (8bit) 
        0x95,0x40, // REPORT_COUNT (64Byte) 
        0x91,0x02, // OUTPUT (Data,Var,Abs) 
 
        0xc0 /* END_COLLECTION */
  }; /* CustomHID_ReportDescriptor */

/* USB String Descriptors (optional) */
const uint8_t CustomHID_StringLangID[CUSTOMHID_SIZ_STRING_LANGID] =
  {
    CUSTOMHID_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const uint8_t CustomHID_StringVendor[CUSTOMHID_SIZ_STRING_VENDOR] =
  {
    CUSTOMHID_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    // Manufacturer: "Idealsee" 
     'I',0,'d',0,'e',0,'a',0,'l',0,'s',0,'e',0,'e',0
  };

const uint8_t CustomHID_StringProduct[CUSTOMHID_SIZ_STRING_PRODUCT] =
  {
   CUSTOMHID_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    //Product Name: IDEALENS k2
    'I', 0, 'D', 0, 'E', 0, 'A', 0,'L', 0, 'E',0,'N',0,'S',0,'_',0,'k',0,'2',0
  };
uint8_t CustomHID_StringSerial[CUSTOMHID_SIZ_STRING_SERIAL] =
  {
     CUSTOMHID_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    //产品序列号
    'M', 0, '1', 0, '0', 0, '0', 0, '0', 0,'V', 0, '0', 0, '1', 0
  };

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


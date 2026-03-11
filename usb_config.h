#ifndef _USB_CONFIG_H
#define _USB_CONFIG_H

#include "funconfig.h"

//Defines the number of endpoints for this device. (Always add one for EP0). For two EPs, this should be 3.
#define ENDPOINTS 2

#define USB_PORT A     // [A,C,D] GPIO Port to use with D+, D- and DPU
#define USB_PIN_DP 1   // [0-4] GPIO Number for USB D+ Pin
#define USB_PIN_DM 2   // [0-4] GPIO Number for USB D- Pin
// #define USB_PIN_DPU 5  // [0-7] GPIO for feeding the 1.5k Pull-Up on USB D- Pin; Comment out if not used / tied to 3V3!

#define RV003USB_DEBUG_TIMING      0
#define RV003USB_OPTIMIZE_FLASH    1
#define RV003USB_EVENT_DEBUGGING   0
#define RV003USB_HANDLE_IN_REQUEST 1
#define RV003USB_OTHER_CONTROL     1
#define RV003USB_HANDLE_USER_DATA  0
#define RV003USB_HID_FEATURES      1
#define RV003USB_USE_REBOOT_FEATURE_REPORT 0

#ifndef __ASSEMBLER__
#ifdef INSTANCE_DESCRIPTORS
/* USB Device Descriptor */
typedef struct __packed {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint16_t bcdUSB;
  uint8_t bDeviceClass;
  uint8_t bDeviceSubClass;
  uint8_t bDeviceProtocol;
  uint8_t bMaxPacketSize0;
  uint16_t idVendor;
  uint16_t idProduct;
  uint16_t bcdDevice;
  uint8_t iManufacturer;
  uint8_t iProduct;
  uint8_t iSerialNumber;
  uint8_t bNumConfigurations;
} USB_DEV_DESCR;

static const USB_DEV_DESCR u2hts_device_desc = {
    .bLength = sizeof(USB_DEV_DESCR),
    .bDescriptorType = 1,  // Device
    .bcdUSB = 0x0100,
    .bDeviceClass = 0x00,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = 8,  // EP0 size
    .idVendor = 0x1A86,
    .idProduct = 0x8572,
    .bcdDevice = 0x0000,
    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,
    .bNumConfigurations = 1};

#define U2HTS_HID_TP                                                           \
      0x09, 0x22, /* (LOCAL)  USAGE              Touch Point */            \
      0xA1, 0x02, /* (MAIN)   COLLECTION         Logical */                    \
      0x09, 0x42, /* (LOCAL)  USAGE              Tip Switch */                 \
      0x25, 0x01, /* (GLOBAL) LOGICAL_MAXIMUM    1 */                          \
      0x75, 0x01, /* (GLOBAL) REPORT_SIZE        1 bit */                      \
      0x95, 0x01, /* (GLOBAL) REPORT_COUNT       1 */                          \
      0x81, 0x02, /* (MAIN)   INPUT              (Data, Variable, Absolute) */ \
      0x09, 0x51, /* (LOCAL)  USAGE              Contact Identifier */         \
      0x75, 0x07, /* (GLOBAL) REPORT_SIZE        7 bits */                     \
      0x81, 0x02, /* (MAIN)   INPUT              (Data, Variable, Absolute) */ \
      0x05, 0x01, /* (GLOBAL) USAGE_PAGE         Generic Desktop */            \
      0x26, 0xFF, 0x0F, /* (GLOBAL) LOGICAL_MAXIMUM    4095 */                 \
      0x75, 0x0C,       /* (GLOBAL) REPORT_SIZE        12 bits */              \
      0x09, 0x30,       /* (LOCAL)  USAGE              X */                    \
      0x46, 0xFF, 0x0F, /* (GLOBAL) PHYSICAL_MAXIMUM   4095 */                 \
      0x81, 0x02, /* (MAIN)   INPUT              (Data, Variable, Absolute) */ \
      0x46, 0xFF, 0x0F, /* (GLOBAL) PHYSICAL_MAXIMUM   4095 */                 \
      0x09, 0x31,       /* (LOCAL)  USAGE              Y */                    \
      0x81, 0x02, /* (MAIN)   INPUT              (Data, Variable, Absolute) */ \
      0x05, 0x0D, /* (GLOBAL) USAGE_PAGE         Digitizers */                 \
      0xC0        /* (MAIN)   END_COLLECTION */

static const uint8_t u2hts_hid_report_desc[] = {
0x05, 0x0D,                    // (GLOBAL) USAGE_PAGE         Digitizers
    0x09, 0x04,                    // (LOCAL)  USAGE              Touch Screen
    0xA1, 0x01,                    // (MAIN)   COLLECTION         Application
    0x85, 1,  // (GLOBAL) REPORT_ID
    0x09, 0x22,                    // (LOCAL)  USAGE              Touch Point
    0x34,                          // (GLOBAL) PHYSICAL_MINIMUM   0
    0x14,                          // (GLOBAL) LOGICAL_MINIMUM    0
    0x55, 0x0E,                    // (GLOBAL) UNIT_EXPONENT      -2
    0x65, 0x11,                    // (GLOBAL) UNIT               SI Linear (Centimeter)
    U2HTS_HID_TP,                  // Touch Point 1
    U2HTS_HID_TP,                  // Touch Point 2
    U2HTS_HID_TP,                  // Touch Point 3
    U2HTS_HID_TP,                  // Touch Point 4
    U2HTS_HID_TP,                  // Touch Point 5
    // U2HTS_HID_TP,                  // Touch Point 6
    // U2HTS_HID_TP,                  // Touch Point 7
    // U2HTS_HID_TP,                  // Touch Point 8
    // U2HTS_HID_TP,                  // Touch Point 9
    // U2HTS_HID_TP,                  // Touch Point 10
    0x27, 0xFF, 0xFF, 0x00, 0x00,  // (GLOBAL) LOGICAL_MAXIMUM    65535
    0x75, 0x10,                    // (GLOBAL) REPORT_SIZE        16 bits
    0x55, 0x0C,                    // (GLOBAL) UNIT_EXPONENT      -4
    0x66, 0x01, 0x10,              // (GLOBAL) UNIT               SI Linear (Seconds)
    0x95, 0x01,                    // (GLOBAL) REPORT_COUNT       1
    0x09, 0x56,                    // (LOCAL)  USAGE              Scan Time
    0x81, 0x02,                    // (MAIN)   INPUT              (Data, Variable, Absolute)
    0x09, 0x54,                    // (LOCAL)  USAGE              Contact Count
    0x25, 0x0A,                    // (GLOBAL) LOGICAL_MAXIMUM    10
    0x75, 0x08,                    // (GLOBAL) REPORT_SIZE        8 bits
    0x81, 0x02,                    // (MAIN)   INPUT              (Data, Variable, Absolute)
    0x85, 2, // (GLOBAL) REPORT_ID
    0x09, 0x55,                    // (LOCAL)  USAGE              Contact Count Maximum
    0xB1, 0x02,                    // (MAIN)   FEATURE            (Data, Variable, Absolute)
    0xC0
};

static const uint8_t config_descriptor[] = {
    // configuration descriptor, USB spec 9.6.3, page 264-266, Table 9-10
    9,  // bLength;
    2,  // bDescriptorType;
    34, 0x00, //for just the one descriptor

    0x01,  // bNumInterfaces (Normally 1)
    0x01,  // bConfigurationValue
    0x00,  // iConfiguration
    0x80,  // bmAttributes (was 0xa0)
    0x64,  // bMaxPower (200mA)

    // Interface descriptor
    9,     // bLength
    4,     // bDescriptorType
    0,     // bInterfaceNumber  = 1 instead of 0 -- well make it second.
    0,     // bAlternateSetting
    1,     // bNumEndpoints
    0x03,  // bInterfaceClass (0x03 = HID)
    0x00,  // bInterfaceSubClass
    0x00,  // bInterfaceProtocol (1 = Keyboard, 2 = Mouse)
    0,     // iInterface
    // hid descriptor
    9,     // bLength
    0x21,  // bDescriptorType (HID)
    0x10,
    0x01,  // bcd 1.1
    0x00,  // country code
    0x01,  // Num descriptors
    0x22,  // DescriptorType[0] (HID)
    sizeof(u2hts_hid_report_desc) & 0xFF,
    sizeof(u2hts_hid_report_desc) >> 8,
    // endpoint descriptor
    7,     // endpoint descriptor (For endpoint 1)
    0x05,  // Endpoint Descriptor (Must be 5)
    0x81,  // Endpoint Address
    0x03,  // Attributes
    0x08,
    0x00,  // Size
    0x01,  // Interval
};

#define STR_MANUFACTURER u"WCH"
#define STR_PRODUCT      u"USB to HID Touchscreen"
#ifndef STR_SERIAL
#define STR_SERIAL       u"USBTOHIDTOUCHSCREEN"
#endif

struct usb_string_descriptor_struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wString[];
};
const static struct usb_string_descriptor_struct string0 __attribute__((section(".rodata"))) = {
	4,
	3,
	{0x0409}
};
const static struct usb_string_descriptor_struct string1 __attribute__((section(".rodata")))  = {
	sizeof(STR_MANUFACTURER),
	3,
	STR_MANUFACTURER
};
const static struct usb_string_descriptor_struct string2 __attribute__((section(".rodata")))  = {
	sizeof(STR_PRODUCT),
	3,
	STR_PRODUCT
};
const static struct usb_string_descriptor_struct string3 __attribute__((section(".rodata")))  = {
	sizeof(STR_SERIAL),
	3,
	STR_SERIAL
};

// This table defines which descriptor data is sent for each specific
// request from the host (in wValue and wIndex).
const static struct descriptor_list_struct {
	uint32_t	lIndexValue;
	const uint8_t	*addr;
	uint16_t		length;
} descriptor_list[] = {
	{0x00000100, (uint8_t *)&u2hts_device_desc, sizeof(u2hts_device_desc)},
	{0x00000200, config_descriptor, sizeof(config_descriptor)},
	// interface number // 2200 for hid descriptors.
	{0x00002200, u2hts_hid_report_desc, sizeof(u2hts_hid_report_desc)},
	{0x00002100, config_descriptor + 18, 9 }, // Not sure why, this seems to be useful for Windows + Android.

	{0x00000300, (const uint8_t *)&string0, 4},
	{0x04090301, (const uint8_t *)&string1, sizeof(STR_MANUFACTURER)},
	{0x04090302, (const uint8_t *)&string2, sizeof(STR_PRODUCT)},	
	{0x04090303, (const uint8_t *)&string3, sizeof(STR_SERIAL)}
};
#define DESCRIPTOR_LIST_ENTRIES ((sizeof(descriptor_list))/(sizeof(struct descriptor_list_struct)) )

#endif // INSTANCE_DESCRIPTORS

#endif

#endif 

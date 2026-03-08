#ifndef _U2HTS_V003J4_H_
#define _U2HTS_V003J4_H_
#include "ch32fun.h"
#include "u2hts_core.h"
#define U2HTS_SWAP16(x) __builtin_bswap16(x)

#define U2HTS_SWAP32(x) __builtin_bswap32(x)

#define UNUSED(x) (void)(x)

// I2C
#define U2HTS_I2C_PORT GPIOC
#define U2HTS_I2C_SDA GPIO_Pin_2
#define U2HTS_I2C_SCL GPIO_Pin_1

#define U2HTS_TP_PORT GPIOC
#define U2HTS_TP_RST GPIO_Pin_4

inline static void u2hts_pins_init() {
    funGpioInitC();
    funPinMode(PC4, GPIO_Speed_30MHz | GPIO_CNF_OUT_OD);
}

#endif
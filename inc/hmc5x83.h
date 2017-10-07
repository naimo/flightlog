/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HMC5X83_H
#define __HMC5X83_H

#define HMC_I2C_ADDRESS 0x3C
#define HMC_REG_CONFIGA 0x00
#define HMC_REG_CONFIGB 0x01

#define HMC_REG_MODE 0x02

#define HMC_REG_IDA 0x0A
#define HMC_REG_IDB 0x0B
#define HMC_REG_IDC 0x0C

#define HMC_REG_DATAOUT 0x03

// #define HMC_REG_TEMPOUT 0x31

// #define HMC_CONFIGA_TS 1 << 7
#define HMC_CONFIGA_MA_1 0 << 5
#define HMC_CONFIGA_MA_2 1 << 5
#define HMC_CONFIGA_MA_4 2 << 5
#define HMC_CONFIGA_MA_8 3 << 5
#define HMC_CONFIGA_DO_0_75 0 << 2
#define HMC_CONFIGA_DO_15 4 << 2
#define HMC_CONFIGA_MS_NORMAL 0
// #define HMC_CONFIGA_MS_TEMP 3

#define HMC_CONFIGB_GAIN_0_88 0 << 5
#define HMC_CONFIGB_GAIN_1_3 1 << 5
#define HMC_CONFIGB_GAIN_4_0 4 << 5

#define HMC_MODE_MD_CONTINOUS 0

// void MPU_I2C_Init();
void HMC_Init();
void HMC_Read();

#endif
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MS5611_H
#define __MS5611_H

#define MS5611_I2C_ADDRESS 0xEE

#define MS5611_CMD_RESET 0x1E
#define MS5611_CMD_PROM 0xA0
#define MS5611_CMD_ADC 0x00

#define MS5611_CMD_D1_4096 0x48
#define MS5611_CMD_D2_256 0x50
#define MS5611_CMD_D2_4096 0x58


typedef struct MS5611_Calib_s {
    unsigned short factory;
    unsigned short C1;
    unsigned short C2;
    unsigned short C3;
    unsigned short C4;
    unsigned short C5;
    unsigned short C6;
} MS5611_Calib_t;

void MS5611_Init();
void MS5611_Read_Acc_Gyro();
void MS5611_Read_Temp();
void MS5611_Read_Temp_and_Pressure();

#endif
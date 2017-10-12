/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MS5607_H
#define __MS5607_H

#define MS5607_I2C_ADDRESS 0xEE

#define MS5607_CMD_RESET 0x1E
#define MS5607_CMD_PROM 0xA0
#define MS5607_CMD_ADC 0x00

#define MS5607_CMD_D1_256 0x40
#define MS5607_CMD_D1_4096 0x48
#define MS5607_CMD_D2_256 0x50
#define MS5607_CMD_D2_4096 0x58


typedef struct MS5607_Calib_s {
    uint16_t factory;
    uint16_t C1;
    uint16_t C2;
    uint16_t C3;
    uint16_t C4;
    uint16_t C5;
    uint16_t C6;
} MS5607_Calib_t;

void MS5607_Init();
void MS5607_Read_Acc_Gyro();
void MS5607_Read_Temp_and_Pressure(int32_t *temp, int32_t *pressure);

#endif
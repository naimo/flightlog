#include <stdio.h>
#include "ms5611.h"
#include "i2c.h"
#include "stm32f3xx_ll_utils.h"

static MS5611_Calib_t MS5611_Calib;

void MS5611_Send_Command(char cmd){
    I2C_Send_Command(MS5611_I2C_ADDRESS, cmd);
}

void MS5611_Write_Register(char reg, char value){
    I2C_Write_Register(MS5611_I2C_ADDRESS, reg, value);
}

char MS5611_Read_Register(char reg){
    return I2C_Read_Register(MS5611_I2C_ADDRESS, reg);
}

void MS5611_Burst_Read_Registers(char reg, int number, char* result)
{
    I2C_Burst_Read_Registers(MS5611_I2C_ADDRESS, reg, number, result);
}

void MS5611_Init()
{    
    LL_mDelay(20);    

    //reset MS5611    
    MS5611_Send_Command(MS5611_CMD_RESET);

    LL_mDelay(20);        

    // read calibration coefficients (PROM base + i = 1..6)
    for (int i=0; i<7; i++){
        MS5611_Burst_Read_Registers(MS5611_CMD_PROM + (i<<1), 2, ((char*)&MS5611_Calib)+2*i);
    }
    
    printf("C1=%5d,C2=%5d,C3=%5d,C4=%5d,C5=%5d,C6=%5d\r\n",   \
                MS5611_Calib.C1,                        \
                MS5611_Calib.C2,                        \
                MS5611_Calib.C3,                        \
                MS5611_Calib.C4,                        \
                MS5611_Calib.C5,                        \
                MS5611_Calib.C6);
    
}

void MS5611_Read_Temp()
{
    char temp_adc[3];
    int temp_adc_combined;
    int dt;
    int temp;

    // start conversion
    MS5611_Send_Command(MS5611_CMD_D2_4096);    

    LL_mDelay(20);    
    
    // start writing at second byte because response is 24bits
    MS5611_Burst_Read_Registers(MS5611_CMD_ADC, 3, temp_adc);

    printf("temp_adc_0 = %x, temp_adc_1 = %x, temp_adc_2 = %x\r\n", \
                    temp_adc[0], temp_adc[1], temp_adc[2]);    

    temp_adc_combined =                             \
            ((unsigned char)temp_adc[0] << 16) +    \
            ((unsigned char)temp_adc[1]<<8) +       \
            (unsigned char)temp_adc[2];
    dt = temp_adc_combined - (MS5611_Calib.C5 << 8);
    temp = (2000 + dt * MS5611_Calib.C6) >> 23;
    printf("temp_adc_combined = %d, dt = %d, temp = %d\r\n", temp_adc_combined, dt, temp);
}

void MS5611_Read_Temp_and_Pressure()
{
    char temp_pressure[3];
    int pressure_adc_combined;
    int offset;
    int sens;
    int pressure;

    // start conversion
    MS5611_Send_Command(MS5611_CMD_D1_4096);    

    LL_mDelay(20);    
    
    // start writing at second byte because response is 24bits
    MS5611_Burst_Read_Registers(MS5611_CMD_ADC, 3, temp_pressure);

    printf("temp_pressure_0 = %x, temp_pressure_1 = %x, temp_pressure_2 = %x\r\n", \
                    temp_pressure[0], temp_pressure[1], temp_pressure[2]);    

    offset = MS5611_Calib.C2 << 16 /* + dt ... */;
    sens = MS5611_Calib.C1 << 15 /* + dt ... */;
    pressure_adc_combined =                         \
        ((unsigned char)temp_pressure[0] << 16) +   \
        ((unsigned char)temp_pressure[1] << 8) +    \
        (unsigned char)temp_pressure[2];
    pressure = (((pressure_adc_combined * sens) >> 21) - offset ) >> 15;

    printf("pressure_adc_combined = %d, pressure = %d\r\n", pressure_adc_combined, pressure);
}
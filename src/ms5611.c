#include <stdio.h>
#include "ms5611.h"
#include "i2c.h"
#include "stm32f3xx_ll_utils.h"

static MS5611_Calib_t MS5611_Calib;

void MS5611_Send_Command(unsigned char cmd){
    I2C_Send_Command(MS5611_I2C_ADDRESS, cmd);
}

void MS5611_Write_Register(unsigned char reg, unsigned char value){
    I2C_Write_Register(MS5611_I2C_ADDRESS, reg, value);
}

char MS5611_Read_Register(unsigned char reg){
    return I2C_Read_Register(MS5611_I2C_ADDRESS, reg);
}

void MS5611_Burst_Read_Registers(unsigned char reg, int number, unsigned char* result)
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
    unsigned char temp[2];
    int i;
    for (i=0; i<7; i++){
        MS5611_Burst_Read_Registers(MS5611_CMD_PROM + (i<<1), 2, temp);
        *((short*)&MS5611_Calib + i) = temp[0] << 8 | temp[1];
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
    unsigned char temp_adc[3];
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

    temp_adc_combined =              \
            (temp_adc[0] << 16) |    \
            (temp_adc[1] << 8) |     \
            temp_adc[2];
    dt = temp_adc_combined - (MS5611_Calib.C5 << 8);
    temp = ((dt * MS5611_Calib.C6) >> 23);
    printf("temp_adc_combined = %d, dt = %d, temp = %d\r\n", temp_adc_combined, dt, temp);
}

void MS5611_Read_Temp_and_Pressure()
{
    int temp_pressure = 0;
    // int pressure_adc_combined;
    long offset;
    long sens;
    int pressure;

    // start conversion
    MS5611_Send_Command(MS5611_CMD_D1_4096);    

    LL_mDelay(20);    
    
    // start writing at second byte because response is 24bits
    MS5611_Burst_Read_Registers(MS5611_CMD_ADC, 3, ((unsigned char*)&temp_pressure)+1);
 
    printf("temp_pressure = %x\r\n", temp_pressure);    

    offset = MS5611_Calib.C2 << 16 /* + dt ... */;
    sens = MS5611_Calib.C1 << 15 /* + dt ... */;
    pressure = ((((long)temp_pressure * sens) >> 21) - offset ) >> 15;

    printf("raw_pressure = %d, pressure = %d\r\n", temp_pressure, pressure);
}
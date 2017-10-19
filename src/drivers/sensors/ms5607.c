#include <stdio.h>
#include <stdint.h>
#include "stm32f3xx_ll_utils.h"
#include "ms5607.h"
#include "drivers/i2c.h"

static MS5607_Calib_t MS5607_Calib;

void MS5607_Send_Command(uint8_t cmd){
    I2C_Send_Command(MS5607_I2C_ADDRESS, cmd);
}

void MS5607_Write_Register(uint8_t reg, uint8_t value){
    I2C_Write_Register(MS5607_I2C_ADDRESS, reg, value);
}

char MS5607_Read_Register(uint8_t reg){
    return I2C_Read_Register(MS5607_I2C_ADDRESS, reg);
}

void MS5607_Burst_Read_Registers(uint8_t reg, int32_t number, uint8_t* result)
{
    I2C_Burst_Read_Registers(MS5607_I2C_ADDRESS, reg, number, result);
}

void MS5607_Init()
{    
    LL_mDelay(20);    

    //reset MS5607    
    MS5607_Send_Command(MS5607_CMD_RESET);

    LL_mDelay(20);        

    
    // read calibration coefficients (PROM base + i = 1..6)
    uint8_t temp[2];
    uint32_t i;
    for (i=0; i<7; i++){
        MS5607_Burst_Read_Registers(MS5607_CMD_PROM + (i<<1), 2, temp);
        *((int16_t*)&MS5607_Calib + i) = temp[0] << 8 | temp[1];
    }
    
    printf("C1=%5d,C2=%5d,C3=%5d,C4=%5d,C5=%5d,C6=%5d\r\n",   \
                MS5607_Calib.C1,                        \
                MS5607_Calib.C2,                        \
                MS5607_Calib.C3,                        \
                MS5607_Calib.C4,                        \
                MS5607_Calib.C5,                        \
                MS5607_Calib.C6); 
}

uint32_t MS5607_Read_ADC(){
    uint8_t temp[3];
    // start writing at second byte because response is 24bits
    MS5607_Burst_Read_Registers(MS5607_CMD_ADC, 3, temp);

    return  (uint32_t)((temp[0] << 16) | (temp[1] << 8) | temp[2]);
}


void MS5607_Read_Temp_and_Pressure(int32_t *temp, int32_t *pressure)
{
    uint32_t D1;
    uint32_t D2;
    int32_t dt;

    int64_t offset;
    int64_t sens;
    int32_t _temp;
    int32_t _pressure;

    // start press conversion
    MS5607_Send_Command(MS5607_CMD_D1_4096);    
    LL_mDelay(10); 
    D1 = MS5607_Read_ADC();

    // start temp conversion
    MS5607_Send_Command(MS5607_CMD_D2_4096);    
    LL_mDelay(10); 
    D2 = MS5607_Read_ADC();
       
    dt = D2 - (uint32_t)MS5607_Calib.C5 * 256;
    offset = (int64_t)MS5607_Calib.C2 * 131072 + (int64_t)MS5607_Calib.C4 * dt / 64;
    sens = (int64_t)MS5607_Calib.C1 * 65536 + (int64_t)MS5607_Calib.C3 * dt / 128;
    _temp = 2000 + (int64_t)dt * (int64_t)MS5607_Calib.C6 / 8388608;

    int32_t temp2 = 0;
    int64_t offset2 = 0;
    int64_t sens2 = 0;

    if (_temp<2000){
        temp2 = (int64_t)dt * (int64_t)dt / 2147483648;
        offset2 = 61 * (int64_t)(_temp - 2000) * (int64_t)(_temp - 2000) / 16;
        sens2 = 2 * (int64_t)(_temp - 2000) * (int64_t)(_temp - 2000);
        if (_temp>=-1500){
            offset2 += 15 * (int64_t)(_temp + 1500) * (int64_t)(_temp + 1500);
            sens2 += 8 * (int64_t)(_temp + 1500) * (int64_t)(_temp + 1500);
        }
    }

    _temp -= temp2;
    offset -= offset2;
    sens -= sens2;

    _pressure = ((int64_t)D1 * sens / 2097152 - offset) / 32768;

    // printf("raw_temp = %lu\r\n", D2);
    // printf("dt = %ld\r\n", dt);
    printf("temp = %ld\r\n", _temp);
    
    // printf("raw_pressure = %lu\r\n", D1);
    // printf("offset = %08lx%08lx, sensitivity = %08lx%08lx\r\n", (uint32_t)(offset>>32), (uint32_t)(offset & 0xFFFFFFFF), (uint32_t)(sens>>32), (uint32_t)(sens & 0xFFFFFFFF));
    printf("pressure = %ld\r\n", _pressure);

    *temp = _temp;
    *pressure = _pressure; 
}
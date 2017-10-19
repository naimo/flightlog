#include <stdio.h>
#include "stm32f3xx_ll_utils.h"
#include "mpu6050.h"
#include "drivers/i2c.h"

void MPU_Write_Register(uint8_t reg, uint8_t value){
    I2C_Write_Register(MPU_I2C_ADDRESS, reg, value);
}

char MPU_Read_Register(uint8_t reg){
    return I2C_Read_Register(MPU_I2C_ADDRESS, reg);
}

void MPU_Burst_Read_Registers(uint8_t reg, int32_t number, uint8_t* result)
{
    I2C_Burst_Read_Registers(MPU_I2C_ADDRESS, reg, number, result);
}

void MPU_Init()
{    
    LL_mDelay(10);    

    //reset MPU
    MPU_Write_Register(MPU_REG_PWR_MGMT_1, 0x80);
    
    LL_mDelay(10);
    
    //set internal 8MHz clock
    MPU_Write_Register(MPU_REG_PWR_MGMT_1, 0x03);

    LL_mDelay(10);    

    //set acc/gyro low pass filter
    MPU_Write_Register(MPU_REG_CONFIG, MPU_CONFIG_DLPF_A5_G5);

    // TODO : CALIB

    LL_mDelay(10);    

    //gyro 500 degrees per second
    MPU_Write_Register(MPU_REG_GYRO_CONFIG, MPU_GYRO_CONFIG_500DPS << 3);

    LL_mDelay(10);    

    //accel +-4g
    MPU_Write_Register(MPU_REG_ACC_CONFIG, MPU_ACC_CONFIG_4G << 3);
}

void MPU_Read_Acc_Gyro()
{
    uint8_t gyro_data[6];
    uint8_t acc_data[6];

    MPU_Burst_Read_Registers(MPU_REG_GYRO_OUT, 6, gyro_data);
    
    printf("gyro x=%5d y=%5d z=%5d ", \
            (int16_t)((gyro_data[0]<<8) | gyro_data[1]),\
            (int16_t)((gyro_data[2]<<8) | gyro_data[3]),\
            (int16_t)((gyro_data[4]<<8) | gyro_data[5]));
    
    MPU_Burst_Read_Registers(MPU_REG_ACC_OUT, 6, acc_data);

    printf("acc x=%5d y=%5d z=%5d\r\n", \
            (int16_t)((acc_data[0]<<8) | acc_data[1]),\
            (int16_t)((acc_data[2]<<8) | acc_data[3]),\
            (int16_t)((acc_data[4]<<8) | acc_data[5]));
}
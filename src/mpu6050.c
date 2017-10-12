#include <stdio.h>
#include "mpu6050.h"
#include "i2c.h"
#include "stm32f3xx_ll_utils.h"

void MPU_Write_Register(unsigned char reg, unsigned char value){
    I2C_Write_Register(MPU_I2C_ADDRESS, reg, value);
}

char MPU_Read_Register(unsigned char reg){
    return I2C_Read_Register(MPU_I2C_ADDRESS, reg);
}

void MPU_Burst_Read_Registers(unsigned char reg, int number, unsigned char* result)
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
    MPU_Write_Register(MPU_REG_GYRO_CONFIG, MPU_GYRO_CONFIG_500DPS);

    LL_mDelay(10);    

    //accel +-4g
    MPU_Write_Register(MPU_REG_ACC_CONFIG, MPU_ACC_CONFIG_4G);
}

void MPU_Read_Acc_Gyro()
{
    unsigned char gyro_data[6];
    unsigned char acc_data[6];

    MPU_Burst_Read_Registers(MPU_REG_GYRO_OUT, 6, gyro_data);
    
    printf("gyro x=%5d y=%5d z=%5d ", \
            (short)((gyro_data[0]<<8) | gyro_data[1]),\
            (short)((gyro_data[2]<<8) | gyro_data[3]),\
            (short)((gyro_data[4]<<8) | gyro_data[5]));
    
    MPU_Burst_Read_Registers(MPU_REG_ACC_OUT, 6, acc_data);

    printf("acc x=%5d y=%5d z=%5d\r\n", \
            (short)((acc_data[0]<<8) | acc_data[1]),\
            (short)((acc_data[2]<<8) | acc_data[3]),\
            (short)((acc_data[4]<<8) | acc_data[5]));
}
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

void MPU_Init(const struct mpu_config *mpuconfig)
{    
    LL_mDelay(10);    

    //reset MPU
    MPU_Write_Register(MPU_REG_PWR_MGMT_1, 0x80);
    
    LL_mDelay(10);
    
    //set internal 8MHz clock
    MPU_Write_Register(MPU_REG_PWR_MGMT_1, 0x03);

    LL_mDelay(10);    

        // acc/gyro low pass filter
    MPU_Write_Register(MPU_REG_CONFIG, mpuconfig->dlpf);

    // TODO : CALIB

    LL_mDelay(10);    

    MPU_Write_Register(MPU_REG_GYRO_CONFIG, mpuconfig->gfs << 3);

    LL_mDelay(10);    

    MPU_Write_Register(MPU_REG_ACC_CONFIG, mpuconfig->afs << 3);
}

void MPU_Read_Acc_Gyro(const struct mpu_config *mpuconfig)
{
    uint8_t gyro_data[6];

    MPU_Burst_Read_Registers(MPU_REG_GYRO_OUT, 6, gyro_data);

    int32_t gyrox,gyroy,gyroz;
    int32_t gyrofs;
    gyrofs = 25000 << (mpuconfig->gfs);
    gyrox = (gyrofs * (int16_t)((gyro_data[0] << 8) | gyro_data[1])) >> 15; 
    gyroy = (gyrofs * (int16_t)((gyro_data[2] << 8) | gyro_data[3])) >> 15;
    gyroz = (gyrofs * (int16_t)((gyro_data[4] << 8) | gyro_data[5])) >> 15;
    printf("gyro x=%6ld y=%6ld z=%6ld (dps/100)\r\n", gyrox, gyroy, gyroz);

    uint8_t acc_data[6];
    
    MPU_Burst_Read_Registers(MPU_REG_ACC_OUT, 6, acc_data);

    int32_t accx,accy,accz;
    int32_t accfs;
    accfs = 20000 << (mpuconfig->afs);
    accx = (accfs * (int16_t)((acc_data[0] << 8) | acc_data[1])) >> 15;
    accy = (accfs * (int16_t)((acc_data[2] << 8) | acc_data[3])) >> 15;
    accz = (accfs * (int16_t)((acc_data[4] << 8) | acc_data[5])) >> 15;

    printf("acc x=%6ld y=%6ld z=%6ld (mg/10)\r\n", accx, accy, accz);
}
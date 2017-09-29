/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MPU6050_H
#define __MPU6050_H

#define MPU_I2C_ADDRESS 0xD0

#define MPU_REG_WHOAMI 0x75
#define MPU_REG_ACC_OUT 0x3B
#define MPU_REG_GYRO_OUT 0x43
#define MPU_REG_PWR_MGMT_1 0x6B
#define MPU_REG_SMPLRT_DIV 0x19
#define MPU_REG_CONFIG 0x1A
#define MPU_REG_GYRO_CONFIG 0x1B
#define MPU_REG_ACC_CONFIG 0x1C


#define MPU_CONFIG_DLPF_A260_G256 0
#define MPU_CONFIG_DLPF_A184_G188 1
#define MPU_CONFIG_DLPF_A10_G10 5
#define MPU_CONFIG_DLPF_A5_G5 6

#define MPU_GYRO_CONFIG_250DPS 0 << 3
#define MPU_GYRO_CONFIG_500DPS 1 << 3
#define MPU_GYRO_CONFIG_1000DPS 2 << 3
#define MPU_GYRO_CONFIG_2000DPS 3 << 3

#define MPU_ACC_CONFIG_2G 0 << 3
#define MPU_ACC_CONFIG_4G 1 << 3
#define MPU_ACC_CONFIG_8G 2 << 3
#define MPU_ACC_CONFIG_16G 3 << 3

void MPU_Init();
void MPU_Read_Acc_Gyro();

#endif
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

enum mpu_config_dlpf {
    MPU_CONFIG_DLPF_A260_G256 = 0,
    MPU_CONFIG_DLPF_A184_G188,
    MPU_CONFIG_DLPF_A94_G98,
    MPU_CONFIG_DLPF_A44_G42,
    MPU_CONFIG_DLPF_A21_G20,
    MPU_CONFIG_DLPF_A10_G10,
    MPU_CONFIG_DLPF_A5_G5
};

enum mpu_gyro_config_gfs {
    MPU_GYRO_CONFIG_250DPS = 0,
    MPU_GYRO_CONFIG_500DPS,
    MPU_GYRO_CONFIG_1000DPS,
    MPU_GYRO_CONFIG_2000DPS
};

enum mpu_acc_config_afs {
    MPU_ACC_CONFIG_2G = 0,
    MPU_ACC_CONFIG_4G,
    MPU_ACC_CONFIG_8G,
    MPU_ACC_CONFIG_16G
};

struct mpu_config {
    enum mpu_config_dlpf dlpf;
    enum mpu_gyro_config_gfs gfs;
    enum mpu_acc_config_afs afs;
};

void MPU_Init();
void MPU_Read_Acc_Gyro();

#endif
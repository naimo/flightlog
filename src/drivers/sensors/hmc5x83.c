#include <stdio.h>
#include "stm32f3xx_ll_utils.h"
#include "hmc5x83.h"
#include "drivers/i2c.h"

void HMC_Write_Register(uint8_t reg, uint8_t value){
    I2C_Write_Register(HMC_I2C_ADDRESS, reg, value);
}

char HMC_Read_Register(uint8_t reg){
    return I2C_Read_Register(HMC_I2C_ADDRESS, reg);
}

void HMC_Burst_Read_Registers(uint8_t reg, int32_t number, uint8_t* result)
{
    I2C_Burst_Read_Registers(HMC_I2C_ADDRESS, reg, number, result);
}

void HMC_Init()
{    
    LL_mDelay(100);    

    HMC_Write_Register(HMC_REG_CONFIGA,         \
                    HMC_CONFIGA_MS_NORMAL |     \
                    HMC_CONFIGA_DO_15 |         \
                 /* HMC_CONFIGA_TS |            \ */
                    HMC_CONFIGA_MA_8            \
                    );
    
    LL_mDelay(10);    
    
    // printf("%x\r\n", HMC_Read_Register(HMC_REG_CONFIGA));

    HMC_Write_Register(HMC_REG_CONFIGB, HMC_CONFIGB_GAIN_0_88);

    LL_mDelay(10);

    HMC_Write_Register(HMC_REG_MODE, HMC_MODE_MD_CONTINOUS);    

    // TODO calib
}

void HMC_Read()
{
    uint8_t mag_data[6];
    // char temp_data[2];
    
    HMC_Burst_Read_Registers(HMC_REG_DATAOUT, 6, mag_data);

    int32_t magx,magy,magz;
    magx = (1000 * (int16_t)((mag_data[0] << 8) | mag_data[1])) / 1370;
    magy = (1000 * (int16_t)((mag_data[2] << 8) | mag_data[3])) / 1370;
    magz = (1000 * (int16_t)((mag_data[4] << 8) | mag_data[5])) / 1370;

    printf("mag x=%5ld y=%5ld z=%5ld (Gauss/1000)\r\n",magx,magy,magz);

    // HMC_Burst_Read_Registers(HMC_REG_TEMPOUT, 2, temp_data);

    // printf("temp=%5d\r\n",(int16_t)((temp_data[0]<<8) + temp_data[1]));
}
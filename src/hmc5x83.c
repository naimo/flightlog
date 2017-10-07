#include <stdio.h>
#include "hmc5x83.h"
#include "i2c.h"
#include "stm32f3xx_ll_utils.h"

void HMC_Write_Register(char reg, char value){
    I2C_Write_Register(HMC_I2C_ADDRESS, reg, value);
}

char HMC_Read_Register(char reg){
    return I2C_Read_Register(HMC_I2C_ADDRESS, reg);
}

void HMC_Burst_Read_Registers(char reg, int number, char* result)
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
    char mag_data[6];
    // char temp_data[2];
    
    HMC_Burst_Read_Registers(HMC_REG_DATAOUT, 6, mag_data);

    printf("mag x=%5d y=%5d z=%5d\r\n", \
    (short)((mag_data[0]<<8) + mag_data[1]),\
    (short)((mag_data[4]<<8) + mag_data[5]),\
    (short)((mag_data[2]<<8) + mag_data[3]));

    // HMC_Burst_Read_Registers(HMC_REG_TEMPOUT, 2, temp_data);

    // printf("temp=%5d\r\n",(short)((temp_data[0]<<8) + temp_data[1]));
}
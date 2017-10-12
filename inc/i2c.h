/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H
#define __I2C_H

void I2C_Init();
void I2C_Send_Command(uint32_t SlaveAddr, uint8_t cmd);
void I2C_Write_Register(uint32_t SlaveAddr, uint8_t reg, uint8_t value);
char I2C_Read_Register(uint32_t SlaveAddr, uint8_t reg);
void I2C_Burst_Read_Registers(uint32_t SlaveAddr, uint8_t reg, int32_t number, uint8_t* result);

#endif
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H
#define __I2C_H

void I2C_Init();
void I2C_Send_Command(uint32_t SlaveAddr, char cmd);
void I2C_Write_Register(uint32_t SlaveAddr, char reg, char value);
char I2C_Read_Register(uint32_t SlaveAddr, char reg);
void I2C_Burst_Read_Registers(uint32_t SlaveAddr, char reg, int number, char* result);

#endif
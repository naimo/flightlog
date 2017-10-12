/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H
#define __I2C_H

void I2C_Init();
void I2C_Send_Command(uint32_t SlaveAddr, unsigned char cmd);
void I2C_Write_Register(uint32_t SlaveAddr, unsigned char reg, unsigned char value);
char I2C_Read_Register(uint32_t SlaveAddr, unsigned char reg);
void I2C_Burst_Read_Registers(uint32_t SlaveAddr, unsigned char reg, int number, unsigned char* result);

#endif
// #include <stdio.h>
#include "stm32f3xx.h"
#include "stm32f3xx_ll_i2c.h"
#include "stm32f3xx_ll_rcc.h"
#include "stm32f3xx_ll_bus.h"
#include "stm32f3xx_ll_gpio.h"

void I2C_Init()
{
    // uint32_t timing = 0;
    
    /* Enbale GPIOB clock */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

    LL_GPIO_InitTypeDef I2C_GPIO_InitStruct;       
    I2C_GPIO_InitStruct.Pin = LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
    I2C_GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    I2C_GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    I2C_GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    I2C_GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    I2C_GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
    LL_GPIO_Init(GPIOB, &I2C_GPIO_InitStruct);

    /* (2) Enable the I2C1 peripheral clock and I2C1 clock source ***************/

    /* Enable the peripheral clock for I2C1 */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

    LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);
    
    /* Disable I2C1 prior modifying configuration registers */
    // LL_I2C_Disable(I2C1);

    /* Configure the SDA setup, hold time and the SCL high, low period */
    /* values from datasheet for 8MHz HSI */
    // timing = __LL_I2C_CONVERT_TIMINGS(0, 0x3, 0x1, 0x3, 0x9);
    // LL_I2C_SetTiming(I2C1, timing);

    LL_I2C_Enable(I2C1);
}

void I2C_Send_Command(uint8_t SlaveAddr, uint8_t cmd)
{
    // Wait for previous communication to end
    while(LL_I2C_IsActiveFlag_BUSY(I2C1));
    
    LL_I2C_HandleTransfer(I2C1, SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);                
    
    LL_I2C_TransmitData8(I2C1, cmd);
    
    return;
}

void I2C_Write_Register(uint8_t SlaveAddr, uint8_t reg, uint8_t value)
{
    // Wait for previous communication to end
    while(LL_I2C_IsActiveFlag_BUSY(I2C1));
    
    LL_I2C_HandleTransfer(I2C1, SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, 2, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);                
    
    LL_I2C_TransmitData8(I2C1, reg);

    while(!LL_I2C_IsActiveFlag_TXIS(I2C1));

    LL_I2C_TransmitData8(I2C1, value);
    
    return;
}

uint8_t I2C_Read_Register(uint8_t SlaveAddr, uint8_t reg)
{
    uint8_t receive;

    // Wait for previous communication to end
    while(LL_I2C_IsActiveFlag_BUSY(I2C1));    
    LL_I2C_HandleTransfer(I2C1, SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);                
    
    LL_I2C_TransmitData8(I2C1, reg);
    
    // Wait for stop
    while(LL_I2C_IsActiveFlag_BUSY(I2C1)){};
    
    // Send read start condition
    LL_I2C_HandleTransfer(I2C1, SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);                

    while(!LL_I2C_IsActiveFlag_RXNE(I2C1));
    receive = LL_I2C_ReceiveData8(I2C1);

    return receive;
}

void I2C_Burst_Read_Registers(uint8_t SlaveAddr, uint8_t reg, int32_t number, uint8_t* result)
{
    
    // Wait for previous communication to end
    while(LL_I2C_IsActiveFlag_BUSY(I2C1));    
    
    // send write start
    LL_I2C_HandleTransfer(I2C1, SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);                
    LL_I2C_TransmitData8(I2C1, reg);  

    // Busy wait for communication to end
    while(LL_I2C_IsActiveFlag_BUSY(I2C1));

    // printf("[I2C]_receive ");
    
    // Send read start condition
    LL_I2C_HandleTransfer(I2C1, SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, number, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);                
    int32_t i;
    for (i=0; i<number; i++){
        while(!LL_I2C_IsActiveFlag_RXNE(I2C1));
        result[i] = LL_I2C_ReceiveData8(I2C1);
        // printf("%x ",result[i]);
    }
    // printf("\r\n");
    
    return;
}
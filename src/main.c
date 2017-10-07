#include <stdio.h>
#include "main.h"
#include "retarget.h"
#include "mpu6050.h"
#include "hmc5983.h"
#include "ms5611.h"
#include "i2c.h"
#include "stm32f3xx.h"
#include "stm32f3xx_ll_bus.h"
#include "stm32f3xx_ll_gpio.h"
#include "stm32f3xx_ll_utils.h"

#define LED_PIN 3

void SystemClock_Config(void);
void StatusLED_Init(void);
void I2C_init();

int main() {
        SystemClock_Config();
        Retarget_Init();
        StatusLED_Init();
        I2C_Init();
        // MPU_Init();
        // HMC_Init();
        
        while(1){
                LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3);

                LL_mDelay(1000);

                MS5611_Init();                                

                LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);

                MS5611_Read_Temp();
                // MPU_Read_Acc_Gyro(); 
                // HMC_Read();             /  
        };
}

void StatusLED_Init(void){
        /* Enbale GPIOB clock */
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

        LL_GPIO_InitTypeDef GPIO_InitStruct;       
        GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
        GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
        LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void SystemClock_Config(void)
{
  /* Set FLASH latency */ 
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Enable HSE and wait for activation*/
  LL_RCC_HSE_Enable(); 
  while(LL_RCC_HSE_IsReady() != 1) 
  {
  };
  
  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
  
  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };
  
  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };
  
   /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2); 
    
  /* Set systick to 1ms in using frequency set to 72MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ (HSE_VALUE, LL_RCC_PLL_MUL_9) */
  LL_Init1msTick(72000000);
  
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(72000000);
}
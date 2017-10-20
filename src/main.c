#include <stdio.h>
#include "main.h"
#include "system/retarget.h"
#include "drivers/sensors/mpu6050.h"
#include "drivers/sensors/hmc5x83.h"
#include "drivers/sensors/ms5607.h"
#include "drivers/i2c.h"
#include "stm32f3xx.h"
#include "stm32f3xx_ll_bus.h"
#include "stm32f3xx_ll_gpio.h"
#include "stm32f3xx_ll_utils.h"

#define LED_PIN 3

void SystemClock_Config(void);
void StatusLED_Init(void);
void I2C_init();

const struct mpu_config mpuconfig = {
        .dlpf = MPU_CONFIG_DLPF_A10_G10,
        //gyro 500 degrees per second      
        .gfs = MPU_GYRO_CONFIG_500DPS,
        //accel +-4g
        .afs = MPU_ACC_CONFIG_4G
};

int main() {
        SystemClock_Config();
        Retarget_Init();
        StatusLED_Init();
        I2C_Init();
        MPU_Init(&mpuconfig);
        HMC_Init();
        MS5607_Init();                                
        
        int32_t temp, pressure;

        while(1){            
                // LL_mDelay(50);
                LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_3);

                MS5607_Read_Temp_and_Pressure(&temp, &pressure);
                MPU_Read_Acc_Gyro(); 
                HMC_Read();
                printf("\r\n");
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
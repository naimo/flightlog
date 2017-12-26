// #include <stdio.h>
#include "stm32f3xx.h"
#include "stm32f3xx_ll_spi.h"
// #include "stm32f3xx_ll_rcc.h"
#include "stm32f3xx_ll_bus.h"
#include "stm32f3xx_ll_gpio.h"
// #include "stm32f3xx_ll_utils.h"

void SPI_Init()
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

    LL_GPIO_InitTypeDef SPI_GPIO_InitStruct;       
    SPI_GPIO_InitStruct.Pin = LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15;
    SPI_GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    SPI_GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    SPI_GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    SPI_GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
    LL_GPIO_Init(GPIOB, &SPI_GPIO_InitStruct);
   
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);

    LL_SPI_Disable(SPI2);
    LL_SPI_DeInit(SPI2);

    LL_SPI_InitTypeDef spi_init;

    spi_init.TransferDirection = LL_SPI_FULL_DUPLEX,
    spi_init.Mode = LL_SPI_MODE_MASTER,
    spi_init.DataWidth = LL_SPI_DATAWIDTH_8BIT,
    spi_init.ClockPolarity = LL_SPI_POLARITY_LOW,
    spi_init.ClockPhase = LL_SPI_PHASE_1EDGE,
    spi_init.NSS = LL_SPI_NSS_HARD_OUTPUT;
    spi_init.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV8;
    spi_init.BitOrder = LL_SPI_MSB_FIRST;
    spi_init.CRCPoly = 7;
    spi_init.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
    LL_SPI_SetRxFIFOThreshold(SPI2, LL_SPI_RX_FIFO_TH_QUARTER);

    LL_SPI_Init(SPI2, &spi_init);
    LL_SPI_Enable(SPI2);
}

void SPI_transfer(const uint8_t *txData, uint8_t *rxData, int len)
{
    while (len--) {
        while(LL_SPI_IsActiveFlag_TXE(SPI2) == 0);
        LL_SPI_TransmitData8(SPI2, *(txData++));

        while(LL_SPI_IsActiveFlag_RXNE(SPI2) == 0);
        *(rxData++) = LL_SPI_ReceiveData8(SPI2);
    }
}
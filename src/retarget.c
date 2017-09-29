// #include "stm32f3xx.h"
#include "stm32f3xx_ll_bus.h"
#include "stm32f3xx_ll_gpio.h"
#include "stm32f3xx_ll_usart.h"

// To use newlib-nano printf

void Retarget_Init()
{
    // Initializes USART 1 TX 115200bps
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

    LL_GPIO_InitTypeDef UART_TXPIN_InitStruct;       
    UART_TXPIN_InitStruct.Pin = LL_GPIO_PIN_9;
    UART_TXPIN_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    UART_TXPIN_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    UART_TXPIN_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    UART_TXPIN_InitStruct.Pull = LL_GPIO_PULL_UP;
    UART_TXPIN_InitStruct.Alternate = LL_GPIO_AF_7;
    LL_GPIO_Init(GPIOA, &UART_TXPIN_InitStruct);

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

    LL_USART_SetTransferDirection(USART1, LL_USART_DIRECTION_TX_RX);
    LL_USART_ConfigCharacter(USART1, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1); 
    LL_USART_SetTransferDirection(USART1, LL_USART_DIRECTION_TX_RX);
    LL_USART_SetBaudRate(USART1, SystemCoreClock/2, LL_USART_OVERSAMPLING_16, 115200);    
    LL_USART_Enable(USART1);

    while((!(LL_USART_IsActiveFlag_TEACK(USART1))) || (!(LL_USART_IsActiveFlag_REACK(USART1))))
    { 
    }
}

int _write(int file, char *ptr, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        while (!LL_USART_IsActiveFlag_TXE(USART1))
        {
        }
        
        LL_USART_TransmitData8(USART1, *ptr++);
    }
    return len;
}
#ifndef __SPI_H
#define __SPI_H

void SPI_Init();
void SPI_transfer(const uint8_t *txData, uint8_t *rxData, int len);

#endif
/**
 * @file bsp_spi.c
 * @brief SPI communication
 */

#include "bsp_spi.h"

void SPI_Enable(void) {
  // LL_SPI_Enable(SPI6);
}

void SPI_TransmitReceive(SPI_TypeDef *SPIx, u8 *TxData, u8 *RxData, u16 Size) {
  if (SPIx == SPI6) {
    extern SPI_HandleTypeDef hspi6;
    HAL_SPI_Transmit(&hspi6, TxData, Size, 1000);
  }
}

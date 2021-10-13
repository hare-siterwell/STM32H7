/**
 * @file bsp_spi.c
 * @brief SPI communication
 */

#include "bsp_spi.h"

void SPI_Enable(void) { LL_SPI_Enable(SPI6); }

void SPI_TransmitReceive(SPI_TypeDef *SPIx, u8 *TxData, u8 *RxData, u16 Size) {
  for (u16 i = 0, timeout = 100; i < Size; i++, timeout = 100) {
    while (!LL_SPI_IsActiveFlag_TXP(SPIx) && timeout--)
      ;
    LL_SPI_TransmitData8(SPIx, *(TxData + i));
    timeout = 100;
    while (!LL_SPI_IsActiveFlag_RXP(SPIx) && timeout--)
      ;
    RxData ? *(RxData + i) = LL_SPI_ReceiveData8(SPIx)
           : LL_SPI_ReceiveData8(SPIx);
  }
}

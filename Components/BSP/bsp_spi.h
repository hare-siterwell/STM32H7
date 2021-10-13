/**
 * @file bsp_spi.h
 * @brief This file contains all the function prototypes for
 *        the bsp_spi.c file
 */

#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_app.h"

void SPI_Enable(void);
void SPI_TransmitReceive(SPI_TypeDef *SPIx, u8 *TxData, u8 *RxData, u16 Size);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_SPI_H */

/**
 * @file bsp_usart.h
 * @brief This file contains all the function prototypes for
 *        the bsp_usart.c file
 */

#ifndef __BSP_USART_H__
#define __BSP_USART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_app.h"

#define BDMA_BUFFER __attribute__((section(".ARM.__at_0x38000000")))

#define USART_RXSIZE 1024u
struct UsartRx {
  OS_SEM sta;           // Received flag
  u16 len;              // Received length
  u8 buf[USART_RXSIZE]; // Received buffer
};
extern struct UsartRx lur1, ur3;

void lpuart1_task(void *p_arg);
void usart3_task(void *p_arg);
void USART_Enable(void);
void USART_RxIdleCallback(USART_TypeDef *USARTx);
void USART_ReEnable(USART_TypeDef *USARTx);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_USART_H__ */
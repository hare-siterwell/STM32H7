/**
 * @file bsp_led.c
 * @brief USART task body
 */

#include "bsp_usart.h"
#include "step.h"

/**
 * @brief 验证数据,驱动步进电机
 * @param kBuf 接收缓冲
 * @param kFormatString 格式字符串
 */
static void verify_buf(uc8 *kBuf, uc8 *kFormatString) {
  s32 step_move = 0;
  u16 motor_id = 0, divnum = 0, step_spmax = 0, step_accel = 0;

  if (sscanf((char *)kBuf, (char *)kFormatString, &step_move, &step_spmax,
             &step_accel, &divnum, &motor_id)) {
    if (motor_id == 1) { // 电机1
      if (motor_move(&motor1, divnum, step_move, step_spmax, step_accel)) {
        printf("Stopped!\r\n");
      } else {
        printf("motorId=%hu,divnum=%hu\r\nstepMove=%d,stepSpmax="
               "%hu,stepAccel=%hu\r\n",
               motor_id, motor1.divnum, motor1.step_move, motor1.step_spmax,
               motor1.step_accel);
      }
    } else {
      printf("Input Error!\r\n");
    }
  } else {
    printf("%s\r\n", kBuf);
  }
}

/**
 * @brief LPUART1 communication with PC
 */
void lpuart1_task(void *p_arg) {
  OS_ERR err;
  OSSemCreate(&lur1.sta, "lur1 sta", 0, &err);
  uc8 kFormatString[] = "\
  {\
    \"step_move\": %d,\
    \"step_spmax\": %hu,\
    \"step_accel\": %hu,\
    \"divnum\": %hu,\
    \"motor_id\": %hu\
  }";
  printf("TaskLpuart1 running!\r\n");
  while (1) {
    OSSemPend(&lur1.sta, 0, OS_OPT_PEND_BLOCKING, 0, &err);

    verify_buf(lur1.buf, kFormatString);

    USART_ReEnable(LPUART1);
  }
}

/**
 * @brief USART3 communication with BLE
 */
void usart3_task(void *p_arg) {
  OS_ERR err;
  uc8 kFormatString[] = "\
  {\
    \"to\": %d,\
    \"sp\": %hu,\
    \"ac\": %hu,\
    \"su\": %hu,\
    \"mo\": %hu\
  }"; // 蓝牙数据传输受限
  motor_arg_init();
  OSSemCreate(&ur3.sta, "ur3 sta", 0, &err);
  printf("TaskUsart3 running!\r\n");

  while (1) {
    OSSemPend(&ur3.sta, 0, OS_OPT_PEND_BLOCKING, 0, &err);

    verify_buf(ur3.buf, kFormatString);

    USART_ReEnable(USART3);
  }
}

/* Support "printf()" */
FILE __stdout;
void _sys_exit(int x) {}
int fputc(int ch, FILE *f) {
  LL_LPUART_TransmitData8(LPUART1, ch);
  while (!LL_LPUART_IsActiveFlag_TXE_TXFNF(LPUART1))
    ;
  return ch;
}

BDMA_BUFFER struct UsartRx lur1;
struct UsartRx ur3;

/**
 * @brief Enable USART
 */
void USART_Enable(void) {
  LL_BDMA_SetPeriphAddress(
      BDMA, LL_BDMA_CHANNEL_0,
      LL_LPUART_DMA_GetRegAddr(LPUART1, LL_LPUART_DMA_REG_DATA_RECEIVE));
  LL_BDMA_SetMemoryAddress(BDMA, LL_BDMA_CHANNEL_0, (u32)lur1.buf);
  LL_BDMA_SetDataLength(BDMA, LL_BDMA_CHANNEL_0, USART_RXSIZE);
  LL_BDMA_EnableChannel(BDMA, LL_BDMA_CHANNEL_0);
  LL_LPUART_EnableDMAReq_RX(LPUART1);
  LL_LPUART_ClearFlag_IDLE(LPUART1);
  LL_LPUART_EnableIT_IDLE(LPUART1);

  LL_BDMA_SetPeriphAddress(
      BDMA, LL_BDMA_CHANNEL_1,
      LL_LPUART_DMA_GetRegAddr(LPUART1, LL_LPUART_DMA_REG_DATA_TRANSMIT));
  LL_LPUART_EnableDMAReq_TX(LPUART1);
}

/**
 * @brief Rx Transfer IDLE callbacks
 * @param USARTx Universal Synchronous Asynchronous Receiver Transmitter
 */
void USART_RxIdleCallback(USART_TypeDef *USARTx) {
  OS_ERR err;
  if (USARTx == LPUART1 && LL_LPUART_IsActiveFlag_IDLE(LPUART1)) {
    LL_LPUART_ClearFlag_IDLE(LPUART1);
    LL_BDMA_DisableChannel(BDMA, LL_BDMA_CHANNEL_0);
    lur1.len = USART_RXSIZE - LL_BDMA_GetDataLength(BDMA, LL_BDMA_CHANNEL_0);
    if (lur1.len) {
      OSSemPost(&lur1.sta, OS_OPT_POST_1, &err); // Processing data
    } else {
      USART_ReEnable(LPUART1);
    }
  } else if (USARTx == USART3 && LL_USART_IsActiveFlag_IDLE(USART3)) {
    LL_USART_ClearFlag_IDLE(USART3);
    LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_0);
    ur3.len = USART_RXSIZE - LL_DMA_GetDataLength(DMA1, LL_DMA_STREAM_0);
    if (ur3.len) {
      OSSemPost(&ur3.sta, OS_OPT_POST_1, &err); // Processing data
    } else {
      USART_ReEnable(USART3);
    }
  }
}

/**
 * @brief Re-enable DMA mode for reception
 * @param USARTx Universal Synchronous Asynchronous Receiver Transmitter
 */
void USART_ReEnable(USART_TypeDef *USARTx) {
  if (USARTx == LPUART1) {
    memset(lur1.buf, 0, lur1.len);
    lur1.len = 0;
    LL_BDMA_SetDataLength(BDMA, LL_BDMA_CHANNEL_0, USART_RXSIZE);
    LL_BDMA_EnableChannel(BDMA, LL_BDMA_CHANNEL_0);
  } else if (USARTx == USART3) {
    memset(ur3.buf, 0, ur3.len);
    ur3.len = 0;
    LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, USART_RXSIZE);
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);
  }
}

/**
 * @brief Sends an amount of data in DMA mode
 * @note Pay attention to bdma access address permissions
 * @param USARTx Universal Synchronous Asynchronous Receiver Transmitter
 * @param pData Pointer to data buffer
 * @param Size Amount of data elements
 */
void USART_Send(USART_TypeDef *USARTx, u8 *pData, u32 Size) {
  if (USARTx == LPUART1) {
    LL_BDMA_DisableChannel(BDMA, LL_BDMA_CHANNEL_1);
    LL_BDMA_SetMemoryAddress(BDMA, LL_BDMA_CHANNEL_1, (u32)pData);
    LL_BDMA_SetDataLength(BDMA, LL_BDMA_CHANNEL_1, Size);
    LL_BDMA_EnableChannel(BDMA, LL_BDMA_CHANNEL_1);
  }
}

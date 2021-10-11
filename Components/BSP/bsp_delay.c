/**
 * @file bsp_delay.c
 * @brief Manage delays
 */

#include "bsp_delay.h"

/**
 * @brief Provide delays in microseconds
 * @param nus Specifies the delay time length in microseconds
 */
void delay_us(u32 nus) {
  uc32 timeout = osKernelGetSysTimerFreq() / 1000000U * nus;
  u32 tcnt, told = osKernelGetSysTimerCount();
  do {
    tcnt = osKernelGetSysTimerCount();
    tcnt = (tcnt > told) ? (tcnt - told) : (UINT32_MAX - told + tcnt);
  } while (tcnt < timeout);
}

/**
 * @brief Provide delays in milliseconds
 * @param nms Specifies the delay time length in milliseconds
 */
void delay_ms(u32 nms) { osDelay(nms); }

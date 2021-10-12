/**
 * @file bsp_delay.c
 * @brief Manage delays
 */

#include "main.h"

/**
 * @brief 初始化延时函数
 * @param SYSCLK 系统时钟频率
 */
void delay_init(void) {
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

/**
 * @brief 延时nus
 * @param nus 需要延时的us数
 */
void delay_us(u32 nus) {
  u32 startTick = DWT->CYCCNT, delayTicks = nus * (SystemCoreClock / 1000000);
  while (DWT->CYCCNT - startTick < delayTicks)
    ;
}

/**
 * @brief 延时nms
 * @param nus 需要延时的ms数
 */
void delay_ms(u32 nms) {
  OS_ERR err;
  OSTimeDly(nms, OS_OPT_TIME_PERIODIC, &err);
}

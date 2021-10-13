/**
 * @file bsp_led.c
 * @brief LED task body
 */

#include "bsp_led.h"

/**
 * @brief Rainbow lights
 */
void led_task(void *p_arg) {
	printf("你好\r\n");
  while (1) {
    LED_R(0);
    LED_B(1);
    delay_ms(500);
    LED_G(0);
    delay_ms(500);
    LED_R(0);
    delay_ms(500);
    LED_B(0);
    delay_ms(500);
    LED_G(1);
    delay_ms(500);
    LED_R(1);
    delay_ms(500);
  }
}

/**
 * @file bsp_lcd.h
 * @brief This file contains all the function prototypes for
 *        the bsp_lcd.c file
 */

#ifndef __BSP_LCD_H
#define __BSP_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_app.h"

void lcd_task(void *p_arg);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_LCD_H */

/**
 * @file bsp_lcd.c
 * @brief LCD task body
 */

#include "bsp_lcd.h"
#include "tftlcd.h"

/**
 * @brief LCD task
 */
void lcd_task(void *p_arg) {
  char str[50] = {0};
  u8 t = 0;

  LCD_Init();
  LCD_Clear(BLACK);
  Display_ALIENTEK_LOGO();
  delay_ms(2000);

  LCD_Clear(BLACK);
  BACK_COLOR = BLACK;
  POINT_COLOR = CYAN;
  LCD_ShowString(0, 0, 160, 12, 12, "Boring_TECH");
  LCD_ShowString(0, 15, 160, 12, 12, "TFTLCD TEST 240*135");
  LCD_ShowString(0, 30, 160, 16, 12, "STM32H743VIT6");

  while (1) {
    sprintf(str, "Time:%4d", t++);
    LCD_ShowString(0, 60, 240, 12, 12, str);
    delay_ms(1000);
  }
}

/**
 * @brief SPI Transmit and Receive
 * @param SPIx SPI
 * @param TxData Transmit Data
 * @param RxData Receive Data
 * @param Size Data size
 */
void SPI_TransmitReceive(SPI_TypeDef *SPIx, u8 *TxData, u8 *RxData, u16 Size) {
  if (SPIx == SPI6) {
    extern SPI_HandleTypeDef hspi6;
    HAL_SPI_Transmit(&hspi6, TxData, Size, 1000);
  }
}

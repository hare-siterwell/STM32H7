/**
 * @file app.c
 * @brief Main thread body
 */

#include "app.h"
#include "stepmotor.h"
#include "tftlcd.h"
#include "usart.h"

__NO_RETURN static void task_lpuart1(void *argument);
__NO_RETURN static void task_usart3(void *argument);
__NO_RETURN static void task_lcd(void *argument);
__NO_RETURN static void task_led(void *argument);

/**
 * @brief The application main thread
 */
void app_main(void *argument) {
  osThreadNew(task_lpuart1, NULL, NULL);
  osThreadNew(task_usart3, NULL, NULL);
  osThreadNew(task_lcd, NULL, NULL);
  osThreadNew(task_led, NULL, NULL);
}

/**
 * @brief 验证数据,驱动步进电机
 * @param kBuf 接收缓冲
 * @param kFormatString 格式字符串
 */
void verify_buf(uc8 *kBuf, uc8 *kFormatString) {
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
__NO_RETURN static void task_lpuart1(void *argument) {
  lur1.sta = osSemaphoreNew(1U, 0U, NULL);
  printf("TaskLpuart1 running!\r\n");
  while (1) {
    osSemaphoreAcquire(lur1.sta, osWaitForever);

    printf("%s\r\nData Length: %d\r\n", lur1.buf, lur1.len);

    USART_ReEnable(LPUART1);
    osThreadYield();
  }
}

/**
 * @brief USART3 communication with BLE
 */
__NO_RETURN static void task_usart3(void *argument) {
  motor_arg_init();

  ur3.sta = osSemaphoreNew(1U, 0U, NULL);
  uc8 kFormatString[] = "\
  {\
    \"to\": %d,\
    \"sp\": %hu,\
    \"ac\": %hu,\
    \"su\": %hu,\
    \"mo\": %hu\
  }"; // 蓝牙数据传输受限

  printf("TaskUsart3 running!\r\n");
  while (1) {
    osSemaphoreAcquire(ur3.sta, osWaitForever);

    verify_buf(ur3.buf, kFormatString);

    USART_ReEnable(USART3);
    osThreadYield();
  }
}

/**
 * @brief LCD
 */
__NO_RETURN static void task_lcd(void *argument) {
  char str[50] = {0};
  u8 t = 0;

  LCD_Init();
  LCD_Clear(BLACK);
  Display_ALIENTEK_LOGO(0, 26);
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
    osThreadYield();
  }
}

/**
 * @brief Rainbow lights
 */
__NO_RETURN static void task_led(void *argument) {
  uc16 nms = 500;
  while (1) {
    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BLUE_GPIO_Port, BLUE_Pin, GPIO_PIN_SET);
    delay_ms(nms);
    HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_RESET);
    delay_ms(nms);
    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_SET);
    delay_ms(nms);
    HAL_GPIO_WritePin(BLUE_GPIO_Port, BLUE_Pin, GPIO_PIN_RESET);
    delay_ms(nms);
    HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_SET);
    delay_ms(nms);
    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_RESET);
    delay_ms(nms);
    osThreadYield();
  }
}

/**
 * @file app.c
 * @brief Main thread body
 */

#include "app.h"
#include "stepmotor.h"
#include "tftlcd.h"
#include "usart.h"

OS_TCB StartTaskTCB;
CPU_STK StartTaskSTK[512];
static OS_TCB Task1TCB;
static CPU_STK Task1Stk[512];
static void lpuart1_task(void *p_arg);
static OS_TCB Task2TCB;
static CPU_STK Task2Stk[512];
static void usart3_task(void *p_arg);
static OS_TCB Task3TCB;
static CPU_STK Task3Stk[512];
static void lcd_task(void *p_arg);
static OS_TCB Task4TCB;
static CPU_STK Task4Stk[512];
static void led_task(void *p_arg);

/**
 * @brief The application main thread
 */
void start_task(void *p_arg) {
  OS_ERR err;

  OSTaskCreate(&Task1TCB, "lpuart1 task", lpuart1_task, 0, 5, &Task1Stk[0],
               512 / 10, 512, 0, 0, 0,
               (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);

  OSTaskCreate(&Task2TCB, "usart3 task", usart3_task, 0, 4, &Task2Stk[0],
               512 / 10, 512, 0, 0, 0,
               (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);

  OSTaskCreate(&Task3TCB, "lcd task", lcd_task, 0, 5, &Task3Stk[0], 512 / 10,
               512, 0, 0, 0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);

  OSTaskCreate(&Task4TCB, "led task", led_task, 0, 5, &Task4Stk[0], 512 / 10,
               512, 0, 0, 0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
}

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
static void lpuart1_task(void *p_arg) {
  OS_ERR err;
  OSSemCreate(&lur1.sta, "lur1 sta", 0, &err);

  printf("TaskLpuart1 running!\r\n");
  while (1) {
    delay_ms(1);
    OSSemPend(&lur1.sta, 0, OS_OPT_PEND_BLOCKING, 0, &err);

    printf("%s\r\nData Length: %d\r\n", lur1.buf, lur1.len);

    USART_ReEnable(LPUART1);
    delay_ms(1);
  }
}

/**
 * @brief USART3 communication with BLE
 */
static void usart3_task(void *p_arg) {
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
    delay_ms(1);
  }
}

/**
 * @brief LCD
 */
static void lcd_task(void *p_arg) {
  // OS_ERR err;
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
 * @brief Rainbow lights
 */
static void led_task(void *p_arg) {
  while (1) {
    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BLUE_GPIO_Port, BLUE_Pin, GPIO_PIN_SET);
    delay_ms(500);
    HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_RESET);
    delay_ms(500);
    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_SET);
    delay_ms(500);
    HAL_GPIO_WritePin(BLUE_GPIO_Port, BLUE_Pin, GPIO_PIN_RESET);
    delay_ms(500);
    HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_SET);
    delay_ms(500);
    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_RESET);
    delay_ms(500);
  }
}

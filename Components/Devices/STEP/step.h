/**
 * @file step.h
 * @brief This file contains all the function prototypes for
 *        the step.c file
 */

#ifndef __STEP_H
#define __STEP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_app.h"

enum MotorState {
  stopped_state,      // 停止状态
  acceleration_state, // 加速状态
  uniform_state,      // 匀速状态
  deceleration_state  // 减速状态
};

struct Motor {
  u32 step_move;  // 需要移动的步数
  u32 step_count; // 已经运行的步数
  u32 step_acced; // 加减速阶段的步数
  u32 step_frac;  // 步数累加器
  u32 speed_frac; // 速度累加器
  u32 step_speed; // 当前速度
  u16 step_spmax; // 移动过程中的最大速度
  u16 step_accel; // 加速度
  u16 divnum;     // 细分

  enum MotorState state; // 运行状态

  GPIO_TypeDef *step_gpio_array, *dir_gpio_array, *ms1_gpio_array,
      *ms2_gpio_array, *ms3_gpio_array; // IO组:脉冲,方向,MS1,MS2,MS3
  u16 step_gpio, dir_gpio, ms1_gpio, ms2_gpio,
      ms3_gpio; // IO号:脉冲,方向,MS1,MS2,MS3

  TIM_TypeDef *tim;
};
extern struct Motor motor1;

void motor_arg_init(void);
int motor_move(struct Motor *motor, u16 divnum, s32 step_move, u16 step_spmax,
               u16 step_accel);
void motor_spta_algorithm(struct Motor *motor);

#ifdef __cplusplus
}
#endif

#endif /* __STEP_H */

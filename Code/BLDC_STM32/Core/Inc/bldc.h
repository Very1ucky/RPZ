#ifndef BLDC_H
#define BLDC_H

#include "main.h"
#include "stm32f4xx_hal.h"
#include "stdint.h"
#include "stdbool.h"
#include "smo.h"

#define L_IN_STATE RESET
#define H_IN_STATE SET

#define PWM_TIM_CP 0

typedef enum {
    W,
    V,
    U
} PwmChannels;

void bldc_start();
void pwm_tim_middle_period_callback();

#endif
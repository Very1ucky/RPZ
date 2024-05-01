#ifndef BLDC_H
#define BLDC_H

//#include "main.h"
#include "stm32f4xx_hal.h"
#include "stdint.h"
#include "stdbool.h"
#include "smo.h"
#include "dtc.h"
#include "motor_data_worker.h"
#include "uart_transmit.h"

#define L_IN_STATE RESET
#define H_IN_STATE SET

#define PWM_TIM_CP 0

typedef enum {
    W,
    V,
    U
} PwmChannels;

typedef enum {
    V1,
    V2,
    V3,
    V4,
    V5,
    V6
} InverterStates;

void bldc_init();
void bldc_blind_start();
void bldc_dtc_step();

#endif
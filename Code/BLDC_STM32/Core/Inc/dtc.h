#ifndef DTC_H
#define DTC_H

#include "app_conf.h"
#include "smo.h"
#include "motor_data_worker.h"
#include "math.h"

void dtc_init();
float calculate_voltage_coef(float ref_speed);

#endif
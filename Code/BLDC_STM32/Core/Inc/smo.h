#ifndef SMO_H
#define SMO_H

#include "app_conf.h"
#include "math.h"
#include "motor_data_worker.h"

#define RELAY_BOUNDS MAX_STATOR_VOLTAGE*0.02

#define PI 3.14159

#define E_FIL_COEF 2*PI*SMO_f_cutoff*T_s/(2*PI*SMO_f_cutoff*T_s+1)
#define W_FIL_COEF T_s*SMO_w_max*MOTOR_p/240

void smo_init();
void estimate_e_albet();
void estimate_theta_e_and_w_m();

float *get_e_est_ptr();
float *get_theta_e_ptr();
float *get_w_m_ptr();

#endif
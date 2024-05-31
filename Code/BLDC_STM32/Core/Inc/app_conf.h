#ifndef APP_CONF_H
#define APP_CONF_H

#define ENABLE_LOG_PRINT 1

#define FLOATS_TRANSFER_COUNT 9

/*motor parameters*/
#define MOTOR_R_s 0.1
#define MOTOR_L_s 0.0225
#define MOTOR_p 4
#define T_s 0.0001
#define MAX_STATOR_VOLTAGE 12

/*smo parameters*/
#define SMO_g 0.9
#define SMO_eta 7.8821e-4
#define SMO_f_cutoff 1200
#define SMO_w_max 10000

/*dtc parameters*/
#define DTC_w_m_K_p 2.385
#define DTC_w_m_K_i 0.05
#define DTC_T_e_K_p 0.01
#define DTC_T_e_K_i 0.09

#define REFERENCE_SPEED 1400.0

#include <stdint.h>

#endif
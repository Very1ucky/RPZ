#include "dtc.h"

static float *w_m;
static float *e_albet;
static float *i_albet;
static float *u_albet;

static float T_e;

static void estimate_T_e();
static float w_m_and_T_e_PI(float ref_speed);

void dtc_init() {
  e_albet = get_e_est_ptr();
  w_m = get_w_m_ptr();
  i_albet = get_i_albet_ptr();
  u_albet = get_u_albet_ptr();
}

float calculate_voltage_coef(float ref_speed) {
    estimate_T_e();
    float PI_out = w_m_and_T_e_PI(ref_speed);
    PI_out = fminf(fmaxf(PI_out, -1), 1);
    return PI_out;
}

static void estimate_T_e() {
  T_e =
      3 / 4 * (e_albet[0] * i_albet[0] / *w_m + e_albet[1] * i_albet[1] / *w_m);
}

static inline float w_m_and_T_e_PI(float ref_speed) {
    static float err = 0;
    static float w_m_int = 0;
    static float T_e_int = 0;
    static float w_m_PI_out = 0;
    static float T_e_PI_out = 0;

    err = ref_speed - *w_m;
    w_m_int += err;
    w_m_PI_out = DTC_w_m_K_p*err+DTC_w_m_K_i*w_m_int;

    err = w_m_PI_out - T_e;
    T_e_int += err;
    T_e_PI_out = DTC_T_e_K_p*err+DTC_T_e_K_i*w_m_int;

    return T_e_PI_out;
}
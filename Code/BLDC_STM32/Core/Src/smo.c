#include "smo.h"

extern uint8_t arr_to_transmit[];

static float *p_hat_theta_e;
static float *p_hat_w_m;

static float hat_e_albet_filt[2];

static float *i_albet;
static float *u_albet;
static float a;
static float b;
static float g = SMO_g;
static float eta = SMO_eta;

static inline void discrete_lowpass_filter(float *p_input, float *p_filtered, float *p_last_filtered, float fil_coef);
static int sgn(float val);
static void relay(bool *relay_state, float val);

void smo_init() {
    p_hat_theta_e = &((float *)arr_to_transmit)[6];
    p_hat_w_m = &((float *)arr_to_transmit)[7];
    i_albet = get_i_albet_ptr();
    u_albet = get_u_albet_ptr();
    a = expf(-MOTOR_R_s/MOTOR_L_s*T_s);
    b = expf(1-a)/MOTOR_R_s;
}

float *get_e_est_ptr() {
    return hat_e_albet_filt;
}

void estimate_e_albet() {

    static float hat_e_albet_filt_last[2];

    static float hat_e_albet_k_pl_1[2];
    static float hat_i_albet_k_pl_1[2];

    static float hat_i_albet_k[2];
    static float hat_e_albet_k[2];

    static float tilde_i_albet_k_min_1[2];
    static float tilde_i_albet_k[2];

    for (uint8_t pos = 0; pos < 2; ++pos) {
        tilde_i_albet_k[pos] = hat_i_albet_k[pos]-i_albet[pos];
        hat_i_albet_k_pl_1[pos] = a*hat_i_albet_k[pos]+b*u_albet[pos]-b*hat_e_albet_k[pos]-eta*sgn(tilde_i_albet_k[pos]);

        hat_e_albet_k_pl_1[pos] = hat_e_albet_k[pos]+g/b*(tilde_i_albet_k[pos]-a*tilde_i_albet_k_min_1[pos]+eta*sgn(tilde_i_albet_k_min_1[pos]));

        discrete_lowpass_filter(&hat_e_albet_k_pl_1[pos], &hat_e_albet_filt[pos], &hat_e_albet_filt_last[pos], E_FIL_COEF);

        tilde_i_albet_k_min_1[pos] = tilde_i_albet_k[pos];

        hat_i_albet_k[pos] = hat_i_albet_k_pl_1[pos];
        hat_e_albet_k[pos] = hat_e_albet_k_pl_1[pos];

        hat_e_albet_filt_last[pos] = hat_e_albet_filt[pos];
    }
}



void estimate_theta_e_and_w_m() {

    static float d = SMO_f_cutoff/(SMO_w_max*MOTOR_p/60);
    static float c = 0;
    static float e_atan2 = 0;

    static bool relay_states[2];
    static int8_t sign = 1;
    static float e_albet_after_sign[2];

    static float last_theta_e = 0;

    static float related_w_m = 0;
    static float last_related_w_m = 0;

    relay(&relay_states[0], hat_e_albet_filt[0]);
    relay(&relay_states[1], hat_e_albet_filt[1]);

    if (relay_states[0]) {
        sign = relay_states[0] && relay_states[1] ? -1 : 1;

    }

    if (sign == 1) {
        e_albet_after_sign[0] = -hat_e_albet_filt[0];
        e_albet_after_sign[1] = hat_e_albet_filt[1];
    } else {
        e_albet_after_sign[0] = hat_e_albet_filt[0];
        e_albet_after_sign[1] = -hat_e_albet_filt[1];
    }

    e_albet_after_sign[0] = e_albet_after_sign[0]*d+e_albet_after_sign[1]*c;
    e_albet_after_sign[1] = e_albet_after_sign[1]*d-e_albet_after_sign[0]*c;

    e_atan2 = atan2f(e_albet_after_sign[0], e_albet_after_sign[1]);
    if (e_atan2 <= 0) {
        e_atan2 += 2*PI;
    }

    *p_hat_theta_e = e_atan2;

    related_w_m = ((*p_hat_theta_e-last_theta_e)/T_s/MOTOR_p*60/2/PI)/(SMO_w_max*MOTOR_p);
    discrete_lowpass_filter(&related_w_m, &related_w_m, &last_related_w_m, W_FIL_COEF);
    c = related_w_m;

    *p_hat_w_m = related_w_m*SMO_w_max*2*PI/60;

    last_theta_e = *p_hat_theta_e;
    last_related_w_m = related_w_m;
}

float *get_theta_e_ptr() {
    return p_hat_theta_e;
}

float *get_w_m_ptr() {
    return p_hat_w_m;
}

static int sgn(float val) {
    return (0 < val) - (val < 0);
} 

static void relay(bool *relay_state, float val) {
    if (!*relay_state && val > RELAY_BOUNDS) {
        *relay_state = true;
    } else if (*relay_state && val < -RELAY_BOUNDS) {
        *relay_state = false;
    }
}

static inline void discrete_lowpass_filter(float *p_input, float *p_filtered, float *p_last_filtered, float fil_coef) {
    *p_filtered = *p_last_filtered+fil_coef*(*p_input-*p_last_filtered);
}
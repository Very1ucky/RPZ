#include "bldc.h"

extern TIM_HandleTypeDef htim1;

bool cur_inv_state[6];

static GPIO_TypeDef *ch_in_port;
static uint16_t ch_in_pin;
static uint32_t tim_channel;

static float *theta_e;

static uint8_t pwm_persentage = 100;

static bool commut_table[][6] = {
    {1, 0, 0, 0, 0, 1},
    {0, 0, 1, 0, 0, 1},
    {0, 1, 1, 0, 0, 0},
    {0, 1, 0, 0, 1, 0},
    {0, 0, 0, 1, 1, 0},
    {1, 0, 0, 1, 0, 0}
};

static inline InverterStates get_new_commut_state(float voltage_coef);
static uint32_t get_pwm_compare_from_persentage(uint8_t persentage);

void enable_xLH(PwmChannels channel, bool is_H_openned);
void disable_xLH(PwmChannels channel, bool is_H_openned);

static void set_commut_step(uint8_t commut_step);

void bldc_init() {
  smo_init();
  dtc_init();
  theta_e = get_theta_e_ptr();
}

void bldc_blind_start() {
  InverterStates commut_step = 0;
  int i = 20000;
  // Motor start
  while (i > 100) {
    DWT_Delay_us(i / 8);
    set_commut_step(commut_step++);
    commut_step %= 6;
    i = i - 5;
  }
}

void bldc_dtc_step() {
  static float voltage_coef = 0;

  estimate_e_albet();
  estimate_theta_e_and_w_m();

  voltage_coef = calculate_voltage_coef(REFERENCE_SPEED);
  pwm_persentage = (uint8_t)fabsf(voltage_coef)*100;
  InverterStates new_state = get_new_commut_state(voltage_coef);
  set_commut_step(new_state);
}

static inline InverterStates get_new_commut_state(float voltage_coef) {
  InverterStates s = 0;
  if (voltage_coef >= 0) {
    if ((*theta_e >= 0 && *theta_e <= PI/6) || *theta_e > 11*PI/6) {
      s = V2;
    } else if (*theta_e > PI/6 && *theta_e <= PI/2) {
      s = V3;
    } else if (*theta_e > PI/2 && *theta_e <= 5*PI/6) {
      s = V4;
    } else if (*theta_e > 5*PI/6 && *theta_e <= 7*PI/6) {
      s = V5;
    } else if (*theta_e > 7*PI/6 && *theta_e <= 3*PI/2) {
      s = V6;
    } else if (*theta_e > 3*PI/2 && *theta_e <= 11*PI/6) {
      s = V1;
    }
  } else {
    if ((*theta_e >= 0 && *theta_e <= PI/6) || *theta_e > 11*PI/6) {
      s = V5;
    } else if (*theta_e > PI/6 && *theta_e <= PI/2) {
      s = V6;
    } else if (*theta_e > PI/2 && *theta_e <= 5*PI/6) {
      s = V1;
    } else if (*theta_e > 5*PI/6 && *theta_e <= 7*PI/6) {
      s = V2;
    } else if (*theta_e > 7*PI/6 && *theta_e <= 3*PI/2) {
      s = V3;
    } else if (*theta_e > 3*PI/2 && *theta_e <= 11*PI/6) {
      s = V4;
    }
  }

  return s;
}

static void set_commut_step(uint8_t commut_step) {

  bool *new_inverter_state = commut_table[commut_step];

  for (uint8_t inv_key_state_pos = 0; inv_key_state_pos < 6; ++inv_key_state_pos) {
    if (new_inverter_state[inv_key_state_pos] != cur_inv_state[inv_key_state_pos]) {
      PwmChannels cur_channel = inv_key_state_pos / 3;
      bool is_H_openned = inv_key_state_pos % 2 == 0;
      if (new_inverter_state[inv_key_state_pos] == true) {
        enable_xLH(cur_channel, is_H_openned);
      } else {
        disable_xLH(cur_channel, is_H_openned);
      }
    }
  }
}

void set_local_vars_from(PwmChannels channel) {
  switch (channel) {
  case W:
    ch_in_port = W_IN_GPIO_Port;
    ch_in_pin = W_IN_Pin;
    tim_channel = W_CH;
    break;
  case V:
    ch_in_port = V_IN_GPIO_Port;
    ch_in_pin = V_IN_Pin;
    tim_channel = V_CH;
    break;
  case U:
    ch_in_port = U_IN_GPIO_Port;
    ch_in_pin = U_IN_Pin;
    tim_channel = U_CH;
    break;
  default:
    return;
  }
}

static inline void enable_pwm() {
  uint32_t new_compare = get_pwm_compare_from_persentage(pwm_persentage);
  __HAL_TIM_SET_COMPARE(&htim1, tim_channel, new_compare);
}

static uint32_t get_pwm_compare_from_persentage(uint8_t persentage) {
  return persentage;
}

static inline void disable_pwm() {
  __HAL_TIM_SET_COMPARE(&htim1, tim_channel, 0);
}

void enable_xLH(PwmChannels channel, bool is_H_openned) {
  set_local_vars_from(channel);

  HAL_GPIO_WritePin(ch_in_port, ch_in_pin, is_H_openned);
  enable_pwm();
  cur_inv_state[channel * 2 + is_H_openned] = true;
}

void disable_xLH(PwmChannels channel, bool is_H_openned) {
  set_local_vars_from(channel);

  if (cur_inv_state[channel * 2 + is_H_openned]) {
    HAL_GPIO_WritePin(ch_in_port, ch_in_pin, is_H_openned);
  } else {
    disable_pwm();
  }
}

void enable_xL(PwmChannels channel) {
  

  HAL_GPIO_WritePin(ch_in_port, ch_in_pin, L_IN_STATE);
  enable_pwm();
  cur_inv_state[channel * 2] = true;
}

void disable_xL(PwmChannels channel) {
  set_local_vars_from(channel);

  if (cur_inv_state[channel * 2 + 1]) {
    HAL_GPIO_WritePin(ch_in_port, ch_in_pin, H_IN_STATE);
  } else {
    disable_pwm();
  }

  cur_inv_state[channel * 2] = false;
}

void enable_xH(PwmChannels channel) {
  set_local_vars_from(channel);

  HAL_GPIO_WritePin(ch_in_port, ch_in_pin, H_IN_STATE);
  enable_pwm();
  cur_inv_state[channel * 2 + 1] = true;
}

void disable_xH(PwmChannels channel) {
  set_local_vars_from(channel);

  if (cur_inv_state[channel * 2]) {
    HAL_GPIO_WritePin(ch_in_port, ch_in_pin, L_IN_STATE);
  } else {
    disable_pwm();
  }

  cur_inv_state[channel * 2 + 1] = false;
}
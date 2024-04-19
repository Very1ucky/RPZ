#include "motor_data_worker.h"

extern ADC_HandleTypeDef hadc1;
extern bool cur_inv_state[6];

uint16_t adc_data[ADC_TRANSFER_COUNT];
static float i;
static float i_abc[3];
static float i_albet[2];
static float u_ab[2];
static float u_albet[2];

#define SQRT_3 1.732

static void clark_transform(float *p_var_ab, float *p_var_albet);
static void extract_motor_data_from_adc();

void start_adc_transfer() {
    HAL_ADC_Start_DMA(&hadc1, adc_data, ADC_TRANSFER_COUNT);
}

void adc_callback() {
    extract_motor_data_from_adc();

    clark_transform(u_ab, u_albet);
    clark_transform(i_abc, i_albet);
}

float *get_i_albet_ptr() {
    return i_albet;
}

float *get_u_albet_ptr() {
    return u_albet;
}

static void extract_motor_data_from_adc() {
    u_ab[0] = adc_data[0]/4096*12.0;
    u_ab[1] = adc_data[1]/4096*12.0;
    i = adc_data[0]/4096*10.0-5;
    for (uint8_t cur_pos = 0; cur_pos < 3; ++cur_pos) {
        if (cur_inv_state[cur_pos*2]) {
            i_abc[cur_pos] = i;
        } else if (cur_inv_state[cur_pos*2+1]) {
            i_abc[cur_pos] = -i;
        } else {
            i_abc[cur_pos] = 0;
        }
    }
}

static void clark_transform(float *p_var_ab, float *p_var_albet) {
    p_var_albet[0] = p_var_ab[0];
    p_var_albet[1] = 1/SQRT_3*(p_var_ab[0]+2*p_var_ab[1]);
}

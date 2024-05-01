#include "motor_data_worker.h"

extern ADC_HandleTypeDef hadc1;
extern bool cur_inv_state[6];

extern uint8_t arr_to_transmit[];

uint16_t adc_data[4];
static float i;
static float *i_abc;
static float i_albet[2];
static float *u_abc;
static float u_albet[2];

#define SQRT_3 1.732

static void clark_transform_full(float *p_var_abc, float *p_var_albet);
static void clark_transform_simple(float *p_var_ab, float *p_var_albet);
static void extract_motor_data_from_adc();

void start_adc_transfer() {
    i_abc = &((float *)arr_to_transmit)[0];
    u_abc = &((float *)arr_to_transmit)[3];
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_data, 4);
}

void parse_adc_data() {
    extract_motor_data_from_adc();

    clark_transform_full(u_abc, u_albet);
    clark_transform_simple(i_abc, i_albet);
}

float *get_i_abc_ptr() {
    return i_abc;
}

float *get_i_albet_ptr() {
    return i_albet;
}

float *get_u_abc_ptr() {
    return u_abc;
}

float *get_u_albet_ptr() {
    return u_albet;
}

static void extract_motor_data_from_adc() {
    u_abc[0] = (float)adc_data[0]/4096*12.0*2;
    u_abc[1] = (float)adc_data[1]/4096*12.0*2;
    u_abc[2] = (float)adc_data[2]/4096*12.0*2;
    i = (float)adc_data[3]/4096*10.0-5;
    //printf("%d\r\n", ((int)(i*10.0)));
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

static void clark_transform_full(float *p_var_abc, float *p_var_albet) {
    p_var_albet[0] = 2/3*(p_var_abc[0]-0.5*p_var_abc[1]-0.5*p_var_abc[2]);
    p_var_albet[1] = 2/3*(p_var_abc[1]*SQRT_3/2-p_var_abc[2]*SQRT_3/2);
}

static void clark_transform_simple(float *p_var_ab, float *p_var_albet) {
    p_var_albet[0] = p_var_ab[0];
    p_var_albet[1] = 1/SQRT_3*(p_var_ab[0]+2*p_var_ab[1]);
}

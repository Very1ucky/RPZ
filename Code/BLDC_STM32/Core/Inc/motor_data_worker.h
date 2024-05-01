#ifndef ADC_DATA_TRANSF_H
#define ADC_DATA_TRANSF_H

#include "bldc.h"
#include "main.h"

void start_adc_transfer();
void parse_adc_data();

float *get_i_abc_ptr();
float *get_i_albet_ptr();

float *get_u_abc_ptr();
float *get_u_albet_ptr();


#endif
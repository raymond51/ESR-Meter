#ifndef __ESR_READING_H__
#define __ESR_READING_H__

//includes
#include <stdbool.h>
#include <math.h>
#include "main.h"

//defines
#define ESR_MAX_OHM_ADC_RAW 440
#define CALIB_POINTS 12
#define MAX_ADC_RAW 2730

//function prototype
void esr_adc_init(ADC_HandleTypeDef handler);
float measure_adc_reading(void);
int adc_reading_raw(void);
float impedance_reading_linear(float adc_reading);
float impedance_reading_cubic(float adc_reading);
void enable_analog_power(void);
void disable_analog_power(void);
void diff(float *data, float *x_diff, int data_size);
int index_search(float *arr, int size, float temp);
float cubic_interp1d(float measured_voltage, float *x, float *y);

#endif // __ESR_READING_H__

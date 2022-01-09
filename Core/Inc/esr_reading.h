#ifndef __ESR_READING_H__
#define __ESR_READING_H__

//includes
#include <stdbool.h>
#include "main.h"

//function prototype
void esr_adc_init(ADC_HandleTypeDef handler);
float measure_adc_reading(void);
float impedance_reading(float adc_reading);
void enable_analog_power(void);
void disable_analog_power(void);

#endif // __ESR_READING_H__

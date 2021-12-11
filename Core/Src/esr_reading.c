#include "esr_reading.h"
#include "display_pages.h"

#define ADC_VOLTAGE 3.3

static ADC_HandleTypeDef hadc;
static uint32_t adc_ResultDMA;
static float adc_reading;

/**
* @brief setup for accurate adc sampling
*/
void measure_adc_reading(void){

	enable_analog_power();
	// ignore initial reading for warm up
	for(int i=0;i<3;i++){
		while (HAL_ADC_Start_DMA(&hadc, &adc_ResultDMA, 1) != HAL_OK){
		}
	}

	//conversion
	adc_reading = (adc_ResultDMA*ADC_VOLTAGE/0x0FFF);
	disable_analog_power();
	write_float_to_screen(adc_reading*10,false,2,50);

}

/**
* @brief Enable analog power
*/
void enable_analog_power(void){
	HAL_GPIO_WritePin(ANALOG_ON_GPIO_Port, ANALOG_ON_Pin, GPIO_PIN_RESET);
	HAL_Delay(10); //Power line settle time
}

/**
* @brief Disable analog power
*/
void disable_analog_power(void){
	HAL_GPIO_WritePin(ANALOG_ON_GPIO_Port, ANALOG_ON_Pin, GPIO_PIN_SET);
	HAL_Delay(10); //Power line settle time
}

void esr_adc_init(ADC_HandleTypeDef handler){
	hadc = handler;
}

/**
* @brief DMA complete transfer callback
*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    // Conversion Complete & DMA Transfer Complete As Well
    // Update with Latest ADC Conversion Result
}

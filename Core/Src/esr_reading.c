#include "esr_reading.h"
#include "display_pages.h"

#define ADC_VOLTAGE 3.3
#define TWENTY_POINT_AVE 20

static ADC_HandleTypeDef hadc;
static volatile uint16_t adc_ResultDMA[2];
static volatile int adcConversionComplete=0; //set by callback
static float adc_reading;

/**
* @brief setup for accurate adc sampling
*/
float measure_adc_reading(void){

	uint16_t acc_adc_ResultDMA=0;

	enable_analog_power();
	// ignore initial reading for warm up
	for(int i=0;i<3;i++){
		//while (HAL_ADC_Start_DMA(&hadc, (uint32_t*) adc_ResultDMA, 2) != HAL_OK){}
		HAL_ADC_Start_DMA(&hadc, (uint32_t*) adc_ResultDMA, 2);
		while(adcConversionComplete == 0){}
		adcConversionComplete = 0;
	}

	//20 point averaging
	for(int i=0;i<TWENTY_POINT_AVE;i++){
			//while (HAL_ADC_Start_DMA(&hadc, (uint32_t*) adc_ResultDMA, 2) != HAL_OK){}
			HAL_ADC_Start_DMA(&hadc, (uint32_t*) adc_ResultDMA, 2);
			while(adcConversionComplete == 0){}
			adcConversionComplete = 0;
			acc_adc_ResultDMA += adc_ResultDMA[1];
	}

	acc_adc_ResultDMA /= TWENTY_POINT_AVE;

	/*conversion*/
	//adc_reading = (adc_ResultDMA[0]*ADC_VOLTAGE/0x0FFF); //sig before LPF
	adc_reading = (acc_adc_ResultDMA*ADC_VOLTAGE/0x0FFF);
	disable_analog_power();

	return adc_reading;
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
	adcConversionComplete = 1;// Conversion Complete & DMA Transfer Complete As Well
    // Update with Latest ADC Conversion Result
}

#include "esr_reading.h"
#include "display_pages.h"

#define ADC_VOLTAGE 3.3
#define TWENTY_POINT_AVE 20

static ADC_HandleTypeDef hadc;
static volatile uint16_t adc_ResultDMA[2];
static volatile int adcConversionComplete=0; //set by callback

/**
* @brief setup for accurate adc sampling
*/
float measure_adc_reading(void){

	float adc_reading;
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
	//disable_analog_power();

	return adc_reading;
}

/**
* @brief convert measured adc to estimated impedance
*/

float impedance_reading_linear(float adc_reading){
	float est_impedance;
	//gain offset calculated based of readings: 2.01V @ 1R, 0.75 @ 10R
	const float gain = -7.14, offset = 15.35;
	est_impedance = gain * adc_reading + offset;
	return est_impedance;
}

/**
* @brief convert measured adc to estimated impedance using cubic spline interpolation curve
*/

float impedance_reading_cubic(float adc_reading){
	float est_impedance;

	float new_meas = 2.8;//TODO - voltage point
	float cal_res_y[CALIB_POINTS] = {0.1, 0.2, 0.3, 0.5, 1, 2, 5, 10, 20};
	float cal_voltage_x[CALIB_POINTS] = {0.7, 0.9, 1, 1.3, 1.7, 2, 2.5, 2.7, 3};

	est_impedance = cubic_interp1d(new_meas, cal_voltage_x, cal_res_y);

	return est_impedance;
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

/**
* @brief
*/
void diff(float *data, float *x_diff, int data_size)
{
        float present_data = *data;
        float next_data = 0.0;

        for (int i = 0; i < data_size - 1; i++)
        {
                next_data = *(++data);
                *(x_diff)++ = next_data - present_data;
                present_data = next_data;
        }
}

int index_search(float *arr, int size, float temp)
{
        int counter = 0;
        for (int i = 0; i < size; i++)
        {
                if (temp > *arr++){counter++;}else{break;}
        }

        return counter;
}

// store only the voltages values from calibration into eeprom

float cubic_interp1d(float measured_voltage, float *x, float *y)
{

        float f0 = 0.0;
        int index = -1;

        float xi1, xi0, yi1, yi0, zi1, zi0, hi1 = 0;

        // store num array of difference
        float x_diff[CALIB_POINTS - 1] = {0};
        float y_diff[CALIB_POINTS - 1] = {0};

        diff(x, x_diff, CALIB_POINTS);
        diff(y, y_diff, CALIB_POINTS);

        // allocate buffer matrices
        float Li[CALIB_POINTS] = {0};
        float Li_1[CALIB_POINTS - 1] = {0};
        float z[CALIB_POINTS] = {0};

        // fill diagonals Li and Li-1 and solve [L][y] = [B]

        Li[0] = sqrt(2 * x_diff[0]);
        Li_1[0] = 0.0;
        float B0 = 0.0; // natural boundary
        float Bi = 0.0;
        z[0] = B0 / Li[0];

        for (int i = 1; i < CALIB_POINTS - 1; i++)
        {
                Li_1[i] = x_diff[i - 1] / Li[i - 1];
                Li[i] = sqrt(2 * (x_diff[i - 1] + x_diff[i]) - Li_1[i - 1] * Li_1[i - 1]);
                Bi = 6 * (y_diff[i] / x_diff[i] - y_diff[i - 1] / x_diff[i - 1]);
                z[i] = (Bi - Li_1[i - 1] * z[i - 1]) / Li[i];

        }

        int i = CALIB_POINTS - 1;
        Li_1[i - 1] = x_diff[i - 1] / Li[i - 1];
        Li[i] = sqrt(2 * x_diff[i - 1] - Li_1[i - 1] * Li_1[i - 1]);
        Bi = 0.0; // natural boundary
        z[i] = (Bi - Li_1[i - 1] * z[i - 1]) / Li[i];

        // solve [L.T][x] = [y]
        i = CALIB_POINTS - 1;
        z[i] = z[i] / Li[i];

        for (int i = CALIB_POINTS - 2; i >= 0; i--)
        {
                z[i] = (z[i] - Li_1[i - 1] * z[i + 1]) / Li[i];
        }

        // find index for bracket within x
        index = index_search(x, CALIB_POINTS, measured_voltage);

        // interpolation brackets
        xi1 = x[index];
        xi0 = x[index - 1];
        yi1 = y[index];
        yi0 = y[index - 1];
        zi1 = z[index];
        zi0 = z[index - 1];
        hi1 = xi1 - xi0;

        // calculate cubic
        if (measured_voltage <= (float)3.0)
        {
                f0 = zi0 / (6 * hi1) * pow(xi1 - measured_voltage, 3) + zi1 / (6 * hi1) * pow(measured_voltage - xi0, 3) + (yi1 / hi1 - zi1 * hi1 / 6) * (measured_voltage - xi0) + (yi0 / hi1 - zi0 * hi1 / 6) * (xi1 - measured_voltage);
        }
        else
        {
                f0 = -1; // err
        }

        return f0;
}

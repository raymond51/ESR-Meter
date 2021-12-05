#include "rotary_sw.h"

//enum

typedef enum rotary_sw_states
{
	IDLE,
	BUTTON_PRESS_EVENT,
	ROTATION_EVENT
}states_rtry;

states_rtry RotaryStates = IDLE;

//variables

static uint32_t tickstart;
static bool button_transistion_state = false;
static uint16_t initial_rotation_pin;

/**
* @brief Determine rotation direction of the rotary switch
*/
void decode_rotary_sw(uint16_t GPIO_Pin){

#ifdef DEBUG_TEST
	//HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_SET);
#endif /* DEBUG */

	RotaryStates = ROTATION_EVENT;
	initial_rotation_pin = GPIO_Pin;

}

/**
* @brief Handle the button on the rotary switch, determines if press is short/long
*/
void decode_rotary_sw_btn(void){


#ifdef DEBUG_TEST
	//HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_RESET);
#endif /* DEBUG */

	RotaryStates = BUTTON_PRESS_EVENT;

}

/**
* @brief Processing of rotary switch events
*/
void process_rotary_sw(void){
	uint32_t elapsed_time = 0;

	/*update the state in other lib i.e displayPages*/
	switch(RotaryStates){
	case IDLE:
		break;
	case BUTTON_PRESS_EVENT:
		HAL_Delay(DEBOUNCE_DELAY_10MS); //debounce duration
		//noise detect
		if(HAL_GPIO_ReadPin(RTRY_SW_EXTI_GPIO_Port, RTRY_SW_EXTI_Pin) == LOW_STATE && !button_transistion_state){
			button_transistion_state = true;
			tickstart = HAL_GetTick(); //capture time elapsed since interrupt
		} else if(HAL_GPIO_ReadPin(RTRY_SW_EXTI_GPIO_Port, RTRY_SW_EXTI_Pin) == HIGH_STATE && button_transistion_state){
			button_transistion_state = false;
			elapsed_time = HAL_GetTick() - tickstart;
			if(elapsed_time > BUTTON_LONG_PRESS_DURATION){
				//long press
				HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_RESET);
			}else{
				//short press
			}
		}else{button_transistion_state = false;}
		RotaryStates = IDLE;
		break;
	case ROTATION_EVENT:
		HAL_Delay(DEBOUNCE_DELAY_5MS); //debounce duration
		if(initial_rotation_pin == RTRY_DT_EXTI_Pin){
			HAL_Delay(DEBOUNCE_DELAY_10MS); //debounce duration
			if(HAL_GPIO_ReadPin(RTRY_DT_EXTI_GPIO_Port, RTRY_DT_EXTI_Pin) == HIGH_STATE){
				//check other line for high
				if(HAL_GPIO_ReadPin(RTRY_CLK_EXTI_GPIO_Port, RTRY_CLK_EXTI_Pin) == HIGH_STATE){
					//rotation counter-clockwise
					HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_RESET);
				}else{
					//noise
					break;
				}
			}else{
				//check other line for low
				if(HAL_GPIO_ReadPin(RTRY_CLK_EXTI_GPIO_Port, RTRY_CLK_EXTI_Pin) == LOW_STATE){
					//rotation counter-clockwise
					HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_RESET);
				}else{
					//noise
					break;
				}
			}
		}else if(initial_rotation_pin == RTRY_CLK_EXTI_Pin){
			HAL_Delay(DEBOUNCE_DELAY_10MS); //debounce duration
			if(HAL_GPIO_ReadPin(RTRY_CLK_EXTI_GPIO_Port, RTRY_CLK_EXTI_Pin) == HIGH_STATE){
				//check other line for high
				if(HAL_GPIO_ReadPin(RTRY_DT_EXTI_GPIO_Port, RTRY_DT_EXTI_Pin) == HIGH_STATE){
					//rotation clockwise
					HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_SET);
				}else{
					//noise
					break;
				}
			}else{
				//check other line for low
				if(HAL_GPIO_ReadPin(RTRY_DT_EXTI_GPIO_Port, RTRY_DT_EXTI_Pin) == LOW_STATE){
					//rotation clockwise
					HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_SET);
				}else{
					//noise
					break;
				}
			}
		}
		RotaryStates = IDLE;
		break;
	default:
		break;
	}

}


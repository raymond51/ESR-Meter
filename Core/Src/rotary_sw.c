#include "rotary_sw.h"

//enum

typedef enum rotary_sw_states
{
	IDLE_STATE,
	BUTTON_PRESS_EVENT,
	ROTATION_EVENT
}states_rtry;


typedef struct
{

	states_rtry_process currState;
	bool flag_processed;

}rotaryProcessStruct;


states_rtry RotaryStates = IDLE_STATE;

static rotaryProcessStruct rotary_struct = {IDLE, true};

//variables

static uint32_t tickstart;
static bool button_transistion_state = false;
static uint16_t initial_rotation_pin;

/**
* @brief Determine rotation direction of the rotary switch
*/
void decode_rotary_sw(uint16_t GPIO_Pin){

	RotaryStates = ROTATION_EVENT;
	initial_rotation_pin = GPIO_Pin;

}

/**
* @brief Handle the button on the rotary switch, determines if press is short/long
*/
void decode_rotary_sw_btn(void){

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
		HAL_Delay(DEBOUNCE_DELAY_5MS); //debounce duration
		//noise detect
		if(HAL_GPIO_ReadPin(RTRY_SW_EXTI_GPIO_Port, RTRY_SW_EXTI_Pin) == LOW_STATE && !button_transistion_state){
			button_transistion_state = true;
			tickstart = HAL_GetTick(); //capture time elapsed since interrupt
		} else if(HAL_GPIO_ReadPin(RTRY_SW_EXTI_GPIO_Port, RTRY_SW_EXTI_Pin) == HIGH_STATE && button_transistion_state){
			button_transistion_state = false;
			elapsed_time = HAL_GetTick() - tickstart;
			if(elapsed_time > BUTTON_LONG_PRESS_DURATION){
				set_rotaryState(LONG_BTN_PRESS); //long press
#ifdef DEBUG_TEST
				HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_RESET);
#endif /* DEBUG */
			}else{
				set_rotaryState(SHORT_BTN_PRESS); //short press
#ifdef DEBUG_TEST
				HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_SET);
#endif /* DEBUG */
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
					set_rotaryState(ROTATION_EVENT_COUNTER_CLOCKWISE); //rotation counter-clockwise
#ifdef DEBUG_TEST
					HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_RESET);
#endif /* DEBUG */
				}else{
					//noise
					break;
				}
			}else{
				//check other line for low
				if(HAL_GPIO_ReadPin(RTRY_CLK_EXTI_GPIO_Port, RTRY_CLK_EXTI_Pin) == LOW_STATE){
					set_rotaryState(ROTATION_EVENT_COUNTER_CLOCKWISE); //rotation counter-clockwise
#ifdef DEBUG_TEST
					HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_RESET);
#endif /* DEBUG */
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
					set_rotaryState(ROTATION_EVENT_CLOCKWISE); //rotation clockwise
#ifdef DEBUG_TEST
					HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_SET);
#endif /* DEBUG */
				}else{
					//noise
					break;
				}
			}else{
				//check other line for low
				if(HAL_GPIO_ReadPin(RTRY_DT_EXTI_GPIO_Port, RTRY_DT_EXTI_Pin) == LOW_STATE){
					set_rotaryState(ROTATION_EVENT_CLOCKWISE); //rotation clockwise
#ifdef DEBUG_TEST
					HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_SET);
#endif /* DEBUG */

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

bool is_rotaryProcessed(void){
	return rotary_struct.flag_processed;
}

int get_rotaryState(void){
	return (int) rotary_struct.currState;
}

void set_rotaryState(states_rtry_process type){
	rotary_struct.currState = type;
	rotary_struct.flag_processed = false;
}

void reset_rotaryStatus(void){
	rotary_struct.currState = IDLE;
	rotary_struct.flag_processed = true;
}

#ifndef __ROTARY_SW_H__
#define __ROTARY_SW_H__

// Includes
#include <stdbool.h>
#include "main.h"
#include "helper.h"

//defines

#define HIGH_STATE 1
#define LOW_STATE 0
#define DEBOUNCE_DELAY_5MS 5
#define DEBOUNCE_DELAY_10MS 10
#define BUTTON_LONG_PRESS_DURATION 1000 //~1s hold

//enums
typedef enum rotary_sw_states_process
{
	IDLE,
	SHORT_BTN_PRESS,
	LONG_BTN_PRESS,
	ROTATION_EVENT_COUNTER_CLOCKWISE,
	ROTATION_EVENT_CLOCKWISE
}states_rtry_process;


// Function prototypes
void decode_rotary_sw(uint16_t GPIO_Pin);
void decode_rotary_sw_btn(void);
void process_rotary_sw(void);
void set_rotaryState(states_rtry_process type);
bool is_rotaryProcessed(void);
int get_rotaryState(void);
void reset_rotaryStatus(void);

#endif // __HELPER_H__

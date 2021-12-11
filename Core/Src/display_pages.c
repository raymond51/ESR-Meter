#include "display_pages.h"

#include "ssd1306.h"
#include "helper.h"

#define INT_DISPLAY_STORAGE 8
#define FLOAT_DISP_PRESCISION 4
#define MOD_FACTOR 10
#define FONT_SMALL_WIDTH 6
#define FONT_LARGE_WIDTH 11


//buffer structure for terminal debug ouput
#define BUFFER_SIZE 10
typedef struct
{
	uint8_t data[BUFFER_SIZE];
	uint8_t *writeIndex;

}ringBuffer;

static ringBuffer terminalBuf = {{0}, 0}; //fixed size to prevent further memory allocation

/**
* @brief ring buffer init
*/
void init_ringBuffer(void){
	terminalBuf.writeIndex = (uint8_t *) terminalBuf.data; //reset pointer
}

/**
* @brief ring buffer storing
*/
void input_ringBuffer(char c){
//check for end of buffer to wrap around

}

/**
* @brief Render and display page for ESR reading
*/
void ESR_PAGE(void){
	ssd1306_Fill(White);
    ssd1306_SetCursor(2,0);
    ssd1306_WriteString("Testing...", Font_11x18, Black);
    ssd1306_SetCursor(2, 18*2);
    ssd1306_WriteString("Font 6x8", Font_6x8, Black);
    ssd1306_UpdateScreen();
}
/**
* @brief Render and display large/small float to screen. Reduced size and resolution float.
* @PARAM[IN] float_holder : float value to be displayed
* PARAM[IN] is_Large_Font :
* PARAM[IN] x_loc : Set X cursor
* PARAM[IN] y_loc : Set Y cursor
*/
void write_float_to_screen(float float_holder, bool is_Large_Font, int x_loc, int y_loc){

	const int three_sf = 99;
	int float_holder_x_10 = (int) (float_holder * 10); //multiply float first before conversion to int for processing
	char int_part[FLOAT_DISP_PRESCISION]; //Hold integer part of float
	char decimal_part[FLOAT_DISP_PRESCISION]; //Hold decimal part of float to 1 dp

	while(float_holder_x_10)
	{
		if(float_holder_x_10>ESR_IMP_MAX){
			/*Display MAX value for float*/
			snprintf(int_part, FLOAT_DISP_PRESCISION, "%d", ESR_IMP_MAX/10);
			snprintf(decimal_part, FLOAT_DISP_PRESCISION, "%d", ESR_IMP_MAX % MOD_FACTOR);
			break;
		}else if(float_holder_x_10<ESR_IMP_MIN){
			snprintf(int_part, FLOAT_DISP_PRESCISION, "%s", "00"); //represent 0
			snprintf(decimal_part, FLOAT_DISP_PRESCISION, "%d", 0);
			break;
		}else if(float_holder_x_10>three_sf){
	    	snprintf(decimal_part, FLOAT_DISP_PRESCISION, "%d", float_holder_x_10 % MOD_FACTOR);
	    }else{
			snprintf(int_part, FLOAT_DISP_PRESCISION, "%d", float_holder_x_10);
			break;
	    }
		float_holder_x_10 /= 10;
	}

	ssd1306_SetCursor(x_loc, y_loc);
	if(is_Large_Font){
		ssd1306_WriteString(int_part, Font_11x18, Black);
		ssd1306_SetCursor(x_loc+(2*FONT_LARGE_WIDTH), y_loc); //new cursor loc
		ssd1306_WriteString(".", Font_11x18, Black); // dp
		ssd1306_SetCursor(x_loc+(3*FONT_LARGE_WIDTH), y_loc);
		ssd1306_WriteString(decimal_part, Font_11x18, Black);
	}else{
		ssd1306_WriteString(int_part, Font_6x8, Black);
		ssd1306_SetCursor(x_loc+(2*FONT_SMALL_WIDTH), y_loc);
		ssd1306_WriteString(".", Font_6x8, Black);
		ssd1306_SetCursor(x_loc+(3*FONT_SMALL_WIDTH), y_loc);
		ssd1306_WriteString(decimal_part, Font_6x8, Black);
	}
	ssd1306_UpdateScreen();
}

void write_int_to_screen(int int_holder, bool is_Large_Font, int x_loc, int y_loc){

	char int_str[INT_DISPLAY_STORAGE];

	snprintf(int_str, INT_DISPLAY_STORAGE, "%d", int_holder);

	ssd1306_SetCursor(x_loc, y_loc);
	if(is_Large_Font){
		ssd1306_WriteString(int_str, Font_11x18, Black);;
	}else{
		ssd1306_WriteString(int_str, Font_6x8, Black);
	}
	ssd1306_UpdateScreen();
}




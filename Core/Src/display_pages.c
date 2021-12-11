#include "display_pages.h"

#include "ssd1306.h"
#include "helper.h"


#define INT_DISPLAY_STORAGE 8
#define FLOAT_DISP_PRESCISION 4
#define MOD_FACTOR 10
#define FONT_SMALL_WIDTH 6
#define FONT_LARGE_WIDTH 11
#define NAVIGATION_HEADER_HEIGHT 14

typedef enum page_states
{
	LOGIN,
	ESR,
	SETTINGS,
	ENUM_END

	/*
	 * LOGIN,
	 * ESR
	 * ESR TABLE
	 * CALIBRATION
	 * DEBUG TERMINAL
	 * SETTINGS
	 * */

}page_states;

//buffer structure for terminal debug ouput
#define BUFFER_SIZE 10
typedef struct
{
	uint8_t data[BUFFER_SIZE];
	uint8_t *writeIndex;

}ringBuffer;

typedef struct
{
	char pageTitle[ENUM_END][BUFFER_SIZE];
	page_states currPage;
	bool navigation_headerFocus;

}navigationHeader;

static ringBuffer terminalBuf = {{0}, 0}; //fixed size to prevent further memory allocation
static navigationHeader mainNavigation = {{"LOGIN","ESR","SETTINGS"}, LOGIN, false}; //fixed size to prevent further memory allocation

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

	switch(mainNavigation.currPage){
	case LOGIN:
		draw_LoginPage();
		break;
	case ESR:
		break;
	case SETTINGS:
		break;
	case ENUM_END:
		//loop back to ESR
		break;
	default:
		break;
	}
	/*
	ssd1306_Fill(White);
    ssd1306_SetCursor(2,0);
    ssd1306_WriteString("Testing...", Font_11x18, Black);
    ssd1306_SetCursor(2, 18*2);
    ssd1306_WriteString("Font 6x8", Font_6x8, Black);
    ssd1306_UpdateScreen();
    */
}

/**
* @brief Render and display splash screen briefly
*/
void ESR_welcomePage(void){
	uint32_t delta;

	ssd1306_Fill(White);

    ssd1306_SetCursor(16,24);
    ssd1306_WriteString("ESR METER", Font_11x18, Black);

	for(delta = 0; delta < 3; delta ++) {
		ssd1306_DrawRectangle(1 + (3*delta),1 + (3*delta) ,SSD1306_WIDTH-1 - (3*delta),SSD1306_HEIGHT-1 - (3*delta), Black);
	}
	ssd1306_UpdateScreen();

	HAL_Delay(3000);
	draw_LoginPage();

}

void draw_LoginPage(void){

	ssd1306_Fill(White);
	draw_navigationBar();

}

void draw_navigationBar(void){

	char buf[BUFFER_SIZE];
	const int edge_offset = 4;

	ssd1306_DrawRectangle(1, 1, SSD1306_WIDTH-1, NAVIGATION_HEADER_HEIGHT , Black);
	/*Navigation header name*/
	ssd1306_SetCursor(edge_offset, edge_offset);
	ssd1306_WriteString("LOGIN", Font_6x8, Black);

	/*Navigation header index*/
	int temp_index_offset = 100;
	ssd1306_SetCursor(temp_index_offset, edge_offset);
	snprintf(buf, BUFFER_SIZE, "%d", edge_offset);
	ssd1306_WriteString(buf, Font_6x8, Black);
	ssd1306_SetCursor((temp_index_offset+=FONT_SMALL_WIDTH), edge_offset);
	ssd1306_WriteString("/", Font_6x8, Black);
	ssd1306_SetCursor((temp_index_offset+=FONT_SMALL_WIDTH), edge_offset);
	snprintf(buf, BUFFER_SIZE, "%d", 5);
	ssd1306_WriteString(buf, Font_6x8, Black);

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




#include "display_pages.h"

#include "ssd1306.h"
#include "helper.h"
#include "esr_reading.h"
#include "rotary_sw.h"

#define INT_DISPLAY_STORAGE 8
#define FLOAT_DISP_PRESCISION 4
#define MOD_FACTOR 10
#define FONT_SMALL_WIDTH 6
#define FONT_SMALL_HEIGHT 8
#define FONT_LARGE_WIDTH 11
#define NAVIGATION_HEADER_HEIGHT 14

typedef enum page_states
{
	LOGIN=1,
	ESR,
	ESR_TABLE,
	CALIBRATION,
	HISTORY_TERMINAL,
	SETTINGS,
	ENUM_PAGE_END

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
	char pageTitle[ENUM_PAGE_END][2*BUFFER_SIZE];
	page_states currPage;
	bool navigation_headerFocus;
	bool is_loginSuccessful;

}navigationHeader;

#define VOLT_HOLD 4
#define CAP_HOLD 8

typedef struct
{
	int cap_param_volt[VOLT_HOLD];
	float cap_param_cap[CAP_HOLD];
	float cap_imp_data[CAP_HOLD][VOLT_HOLD];
}esrTable;

static ringBuffer terminalBuf = {{0}, 0}; //fixed size to prevent further memory allocation
static navigationHeader mainNavigation = {{"LOGIN","ESR", "ESR TABLE","CALIBRATION", "TERMINAL","SETTINGS"}, LOGIN, false, true};
static esrTable esr_Table;

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
* @brief init function
*/

void ESR_INIT(void){
	ESR_init_esr_table();
}

/**
* @brief Render and display page for ESR reading
*/
void ESR_PAGE(void){

	process_rotary();

	switch(mainNavigation.currPage){
	case LOGIN:
		draw_LoginPage();
		break;
	case ESR:
		draw_ESRPage();
		break;
	case ESR_TABLE:
		draw_ESRTable();
		break;
	case CALIBRATION:
		draw_CalibrationPage();
		break;
	case HISTORY_TERMINAL:
		draw_HistoryTerminal();
		break;
	case SETTINGS:
		draw_SettingPage();
		break;
	case ENUM_PAGE_END:
		//loop back to ESR
		break;
	default:
		break;
	}

	reset_rotaryStatus();

}

void process_rotary(void){

	if(is_rotaryProcessed()!=true){

		if(get_rotaryState() == LONG_BTN_PRESS){
			mainNavigation.navigation_headerFocus = true;
		}else if(get_rotaryState() == SHORT_BTN_PRESS){
			mainNavigation.navigation_headerFocus = false;
		}

		if(mainNavigation.navigation_headerFocus){
			if(get_rotaryState() == ROTATION_EVENT_CLOCKWISE){
				if(mainNavigation.currPage<1){
					mainNavigation.currPage = ENUM_PAGE_END - 1;
				}else{
					mainNavigation.currPage -= 1;
				}
			}else if(get_rotaryState() == ROTATION_EVENT_COUNTER_CLOCKWISE){
				if(mainNavigation.currPage> (ENUM_PAGE_END - 1)){
				mainNavigation.currPage = LOGIN;
				}else{
					mainNavigation.currPage += 1;
				}
			}
		}
	}
}

void ESR_init_esr_table(void){
	esrTable esr_Table_holder = {{10,16,25,35}, {4.7,10,22,47,100,220,470,1000},
			{{40,35,29,24},{20,16,14,11},{9,7.5,6.2,5.1},{4.2,3.5,2.9,2.4},{2,1.6,1.4,1.1},{0.9,0.75,0.62,0.51},{0.42,0.35,0.29,0.24},{0.2,0.16,0.14,0.11}}};

	esr_Table = esr_Table_holder;
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
	HAL_Delay(3000);  //display length

}

void draw_LoginPage(void){

	ssd1306_Fill(White);
	draw_navigationBar();

	/*Display Firmware Version*/

	int temp_firm_offset = 80;
	ssd1306_SetCursor(temp_firm_offset, 55);
	ssd1306_WriteString("VER:", Font_6x8, Black);
	write_float_to_screen(VERSION_FIRMWARE,false,temp_firm_offset += (4*FONT_SMALL_WIDTH),55);


	/*Login Logic*/
	int temp_login_offset = 20;
	ssd1306_SetCursor(30, temp_login_offset);
	ssd1306_WriteString("USER:", Font_6x8, Black);
	ssd1306_SetCursor(30, temp_login_offset += FONT_SMALL_HEIGHT);
	ssd1306_WriteString("PASS:", Font_6x8, Black);
	ssd1306_SetCursor(15, temp_login_offset += (2*FONT_SMALL_HEIGHT));
	ssd1306_WriteString("LOGIN", Font_6x8, Black);
	ssd1306_SetCursor(65, temp_login_offset);
	ssd1306_WriteString("REGISTER", Font_6x8, Black);
	ssd1306_UpdateScreen();

}

void draw_ESRPage(void){

	ssd1306_Fill(White);
	draw_navigationBar();

	/*Reading display*/
	ssd1306_SetCursor(4, 25);
	ssd1306_WriteString("READING:", Font_6x8, Black);
	ssd1306_SetCursor(60, 50);
	ssd1306_WriteString("OHM", Font_6x8, Black);
	write_float_to_screen(VERSION_FIRMWARE,true,4,40);

	/*Modes Display*/
	int temp_mode_offset = 25;
	ssd1306_SetCursor(80, temp_mode_offset);
	ssd1306_WriteString("MODE:", Font_6x8, Black);
	ssd1306_SetCursor(90, (temp_mode_offset+=FONT_SMALL_HEIGHT));
	ssd1306_WriteString("CONT.", Font_6x8, White);
	ssd1306_SetCursor(90, (temp_mode_offset+=FONT_SMALL_HEIGHT));
	ssd1306_WriteString("HOLD", Font_6x8, Black);

	ssd1306_UpdateScreen();
}

void draw_ESRTable(void){
	const int TABLE_GAP = 5;

	ssd1306_Fill(White);
	draw_navigationBar();
	/*ESR table*/

	ssd1306_DrawRectangle(1, NAVIGATION_HEADER_HEIGHT + TABLE_GAP, SSD1306_WIDTH-1, 2* NAVIGATION_HEADER_HEIGHT , Black);
	ssd1306_DrawRectangle(1, 2* NAVIGATION_HEADER_HEIGHT + TABLE_GAP, SSD1306_WIDTH-1, 3 * NAVIGATION_HEADER_HEIGHT , Black);
	ssd1306_DrawRectangle(1, 3 * NAVIGATION_HEADER_HEIGHT + TABLE_GAP, SSD1306_WIDTH-1, 4 * NAVIGATION_HEADER_HEIGHT , Black);

	ssd1306_UpdateScreen();
}

void draw_CalibrationPage(void){
	ssd1306_Fill(White);
	draw_navigationBar();
	/*Calibration Page*/
}

void draw_HistoryTerminal(void){
	ssd1306_Fill(White);
	draw_navigationBar();
	/*History Page*/
}

void draw_SettingPage(void){
	ssd1306_Fill(White);
	draw_navigationBar();
	/*Setting Page*/

}

void draw_navigationBar(void){

	char buf[BUFFER_SIZE];
	const int edge_offset = 4;

	ssd1306_DrawRectangle(1, 1, SSD1306_WIDTH-1, NAVIGATION_HEADER_HEIGHT , Black);
	/*Navigation header name*/
	ssd1306_SetCursor(edge_offset, edge_offset);

	if(mainNavigation.navigation_headerFocus){
		ssd1306_WriteString(*(mainNavigation.pageTitle+((int)mainNavigation.currPage)-1), Font_6x8, White);
	}else{
		ssd1306_WriteString(*(mainNavigation.pageTitle+((int)mainNavigation.currPage)-1), Font_6x8, Black);
	}

	/*Navigation header index*/
	int temp_index_offset = 100;
	ssd1306_SetCursor(temp_index_offset, edge_offset);
	snprintf(buf, BUFFER_SIZE, "%d", (int) mainNavigation.currPage);
	ssd1306_WriteString(buf, Font_6x8, Black);
	ssd1306_SetCursor((temp_index_offset+=FONT_SMALL_WIDTH), edge_offset);
	ssd1306_WriteString("/", Font_6x8, Black);
	ssd1306_SetCursor((temp_index_offset+=FONT_SMALL_WIDTH), edge_offset);
	snprintf(buf, BUFFER_SIZE, "%d", (int) (ENUM_PAGE_END-1));
	ssd1306_WriteString(buf, Font_6x8, Black);

	ssd1306_UpdateScreen();

}

/**
* @brief Render and display large/small float to screen. Reduced size and resolution float (XX.XX).
* @PARAM[IN] float_holder : float value to be displayed
* PARAM[IN] is_Large_Font :
* PARAM[IN] x_loc : Set X cursor
* PARAM[IN] y_loc : Set Y cursor
*/
void write_float_to_screen(float float_holder, bool is_Large_Font, int x_loc, int y_loc){

	const int three_sf = 999, two_sf = 99, one_sf = 9, one_digit = 1;
	int float_holder_x_100 = (int) (float_holder * 100); //multiply float first before conversion to int for processing
	int float_holder_x_100_padding = float_holder_x_100;
	char int_part_msb[FLOAT_DISP_PRESCISION]; //Hold integer part of float
	char int_part_lsb[FLOAT_DISP_PRESCISION]; //Hold integer part of float
	char decimal_part_msb[FLOAT_DISP_PRESCISION]; //Hold decimal part of float left point
	char decimal_part_lsb[FLOAT_DISP_PRESCISION]; //Hold decimal part of float right point

	/*Val retrieval*/
	while(float_holder_x_100)
	{
		if(float_holder_x_100>=ESR_IMP_MAX){
			/*Display MAX value for float*/
			snprintf(int_part_msb, FLOAT_DISP_PRESCISION, "%d", ESR_IMP_MAX/100);
			snprintf(int_part_lsb, FLOAT_DISP_PRESCISION, "%d", ESR_IMP_MAX % MOD_FACTOR);
			snprintf(decimal_part_msb, FLOAT_DISP_PRESCISION, "%d", ESR_IMP_MAX % MOD_FACTOR);
			snprintf(decimal_part_lsb, FLOAT_DISP_PRESCISION, "%d", ESR_IMP_MAX % MOD_FACTOR);
			break;
		}else if(float_holder_x_100>=three_sf && float_holder_x_100<ESR_IMP_MAX){
			snprintf(decimal_part_lsb, FLOAT_DISP_PRESCISION, "%d", float_holder_x_100 % MOD_FACTOR);//decimal lsb
		}else if(float_holder_x_100>=two_sf && float_holder_x_100<three_sf){
			if(float_holder_x_100_padding>three_sf){
				snprintf(decimal_part_msb, FLOAT_DISP_PRESCISION, "%d", float_holder_x_100 % MOD_FACTOR);
			}else if(float_holder_x_100_padding>two_sf && float_holder_x_100_padding<=three_sf){
				snprintf(decimal_part_lsb, FLOAT_DISP_PRESCISION, "%d", float_holder_x_100 % MOD_FACTOR); //decimal lsb
			}
		}else if(float_holder_x_100>=one_sf && float_holder_x_100<two_sf){

			if(float_holder_x_100_padding>three_sf){
				snprintf(int_part_lsb, FLOAT_DISP_PRESCISION, "%d", float_holder_x_100 % MOD_FACTOR); //int lsb
			}else if(float_holder_x_100_padding>two_sf && float_holder_x_100_padding<=three_sf){
				snprintf(decimal_part_msb, FLOAT_DISP_PRESCISION, "%d", float_holder_x_100 % MOD_FACTOR);
			}else if(float_holder_x_100_padding>one_sf && float_holder_x_100_padding<=two_sf){
				snprintf(decimal_part_lsb, FLOAT_DISP_PRESCISION, "%d", float_holder_x_100 % MOD_FACTOR);
			}

		}else if(float_holder_x_100>=one_digit && float_holder_x_100<one_sf){

			if(float_holder_x_100_padding>three_sf){
				snprintf(int_part_msb, FLOAT_DISP_PRESCISION, "%d", float_holder_x_100 % MOD_FACTOR); //integer msb
			}else if(float_holder_x_100_padding>two_sf && float_holder_x_100_padding<=three_sf){
				snprintf(int_part_lsb, FLOAT_DISP_PRESCISION, "%d", float_holder_x_100 % MOD_FACTOR); //integer lsb
			}else if(float_holder_x_100_padding>one_sf && float_holder_x_100_padding<=two_sf){
				snprintf(decimal_part_msb, FLOAT_DISP_PRESCISION, "%d", float_holder_x_100 % MOD_FACTOR);
			}else if(float_holder_x_100_padding>=one_digit && float_holder_x_100_padding<=one_sf){
				snprintf(decimal_part_lsb, FLOAT_DISP_PRESCISION, "%d", float_holder_x_100 % MOD_FACTOR);
			}

		}else if(float_holder_x_100<one_digit){
			//exit on zero
			break;
		}

		float_holder_x_100 /= 10;
	}

	/*Padding*/

		if(float_holder_x_100_padding>=two_sf && float_holder_x_100_padding<three_sf){
			//0X.XX
			snprintf(int_part_msb, FLOAT_DISP_PRESCISION, "%s", "0"); //represent 0
		}else if(float_holder_x_100_padding>=one_sf && float_holder_x_100_padding<two_sf){
			//00.XX
			snprintf(int_part_msb, FLOAT_DISP_PRESCISION, "%s", "0"); //represent 0
			snprintf(int_part_lsb, FLOAT_DISP_PRESCISION, "%s", "0"); //represent 0
		}else if(float_holder_x_100_padding>=one_digit && float_holder_x_100_padding<one_sf){
			//00.0X
			snprintf(int_part_msb, FLOAT_DISP_PRESCISION, "%s", "0"); //represent 0
			snprintf(int_part_lsb, FLOAT_DISP_PRESCISION, "%s", "0"); //represent 0
			snprintf(decimal_part_msb, FLOAT_DISP_PRESCISION, "%s", "0"); //represent 0
		}else if(float_holder_x_100_padding<one_digit){
			//zero
			snprintf(int_part_msb, FLOAT_DISP_PRESCISION, "%s", "0"); //represent 0
			snprintf(int_part_lsb, FLOAT_DISP_PRESCISION, "%s", "0"); //represent 0
			snprintf(decimal_part_msb, FLOAT_DISP_PRESCISION, "%s", "0"); //represent 0
			snprintf(decimal_part_lsb, FLOAT_DISP_PRESCISION, "%s", "0"); //represent 0
		}

	ssd1306_SetCursor(x_loc, y_loc);
	if(is_Large_Font){
		ssd1306_WriteString(int_part_msb, Font_11x18, Black);
		ssd1306_SetCursor(x_loc+FONT_LARGE_WIDTH, y_loc); //new cursor loc
		ssd1306_WriteString(int_part_lsb, Font_11x18, Black);
		ssd1306_SetCursor(x_loc+(2*FONT_LARGE_WIDTH), y_loc);
		ssd1306_WriteString(".", Font_11x18, Black); // dp
		ssd1306_SetCursor(x_loc+(3*FONT_LARGE_WIDTH), y_loc);
		ssd1306_WriteString(decimal_part_msb, Font_11x18, Black);
		ssd1306_SetCursor(x_loc+(4*FONT_LARGE_WIDTH), y_loc);
		ssd1306_WriteString(decimal_part_lsb, Font_11x18, Black);
	}else{
		ssd1306_WriteString(int_part_msb, Font_6x8, Black);
		ssd1306_SetCursor(x_loc+FONT_SMALL_WIDTH, y_loc);
		ssd1306_WriteString(int_part_lsb, Font_6x8, Black);
		ssd1306_SetCursor(x_loc+(2*FONT_SMALL_WIDTH), y_loc);
		ssd1306_WriteString(".", Font_6x8, Black);
		ssd1306_SetCursor(x_loc+(3*FONT_SMALL_WIDTH), y_loc);
		ssd1306_WriteString(decimal_part_msb, Font_6x8, Black);
		ssd1306_SetCursor(x_loc+(4*FONT_SMALL_WIDTH), y_loc);
		ssd1306_WriteString(decimal_part_lsb, Font_6x8, Black);
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




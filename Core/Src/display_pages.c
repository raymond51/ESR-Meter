#include "display_pages.h"

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

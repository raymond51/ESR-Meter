#include "display_pages.h"

/**
* @brief Render and display page for ESR reading
*/
void ESR_PAGE(void){
	ssd1306_Fill(White);
    ssd1306_SetCursor(2,0);
    ssd1306_WriteString("Testing...", Font_11x18, Black);
    ssd1306_SetCursor(2, 18*2);
    ssd1306_WriteString("Font 6x8", Font_6x8, White);
    ssd1306_UpdateScreen();
}

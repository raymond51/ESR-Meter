#ifndef __DISPLAY_PAGES_H__
#define __DISPLAY_PAGES_H__

#include <stdio.h>
#include <stdbool.h>

void ESR_PAGE(void);
void init_ringBuffer(void);
void input_ringBuffer(char c);
void write_float_to_screen(float float_holder, bool is_Large_Font, int x_loc, int y_loc);

#endif // __DISPLAY_PAGES_H__

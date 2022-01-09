#ifndef __DISPLAY_PAGES_H__
#define __DISPLAY_PAGES_H__

#include <stdio.h>
#include <stdbool.h>

void ESR_INIT(void);
void ESR_PAGE(void);
void ESR_init_esr_table(void);
void ESR_init_setting_page(void);
void ESR_welcomePage(void);
void process_rotary(void);
void draw_navigationBar(void);
void draw_LoginPage(void);
void draw_ESRPage(void);
void draw_ESRTable(void);
void draw_CalibrationPage(void);
void draw_HistoryTerminal(void);
void draw_SettingPage(void);
void init_ringBuffer(void);
void input_ringBuffer(char c);
void write_float_to_screen(float float_holder, bool is_Large_Font, int x_loc, int y_loc);
void write_int_to_screen(int int_holder, bool is_Large_Font, bool text_color_white,int x_loc, int y_loc);

#endif // __DISPLAY_PAGES_H__

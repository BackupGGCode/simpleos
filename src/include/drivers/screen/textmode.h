#ifndef DRIVERS_SCREEN_TEXTMODE_H
#define DRIVERS_SCREEN_TEXTMODE_H

#include <lib/stdtypes.h>
#include <lib/stddef.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_BROWN 6
#define COLOR_LTGRAY 7
#define COLOR_GRAY 8
#define COLOR_LTBLUE 9
#define COLOR_LTGREEN 10
#define COLOR_LTCYAN 11
#define COLOR_LTRED 12
#define COLOR_LTMAGENTA 13
#define COLOR_YELLOW 14
#define COLOR_WHITE 15

void textmode_init(void);
void textmode_set_colors(uint8_t forecolor, uint8_t backcolor);
void textmode_move_cursor(int32_t x, int32_t y);
void textmode_clear_screen(void);
void printf(const int8_t *fmt, ...);

#endif

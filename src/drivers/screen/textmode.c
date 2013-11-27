#include <drivers/screen/textmode.h>
#include <lib/stdtypes.h>
#include <lib/x86.h>
#include <lib/string.h>
#include <lib/vsprintf.h>

// VGA framebuffer location
uint16_t *screen_memory = (uint16_t *)0xB8000;

// Cursor position
uint8_t cursor_x = 0;
uint8_t cursor_y = 0;

// Cursor attributes
int32_t attrib;

static void put_cursor(void);
static void scroll(void);
static void textmode_put(int8_t c);
static void textmode_write(int8_t *c);

void textmode_init(void)
{
	textmode_set_colors(COLOR_LTGRAY, COLOR_BLACK); // Default colors

	if (((*(volatile uint16_t *)0x410) & 0x30) == 0x30) {
		screen_memory = (uint16_t *)0xB0000; // Monochrome display

		// TODO: Panic
	}

	textmode_clear_screen();
}

void textmode_set_colors(uint8_t forecolor, uint8_t backcolor)
{
	attrib = (backcolor << 4) | (forecolor & 0x0F);
}

void textmode_move_cursor(int32_t x, int32_t y)
{
	cursor_x = x;
	cursor_y = y;

	put_cursor();
}

static void put_cursor(void)
{
	uint16_t position = (cursor_y * SCREEN_WIDTH) + cursor_x;

	outportb(0x3D4, 14);
	outportb(0x3D5, position >> 8);

	outportb(0x3D4, 15);
	outportb(0x3D5, position);
}

void textmode_clear_screen(void)
{
	int32_t i;

	for (i = 0; i < SCREEN_HEIGHT; i++) {
		memsetw(screen_memory + i * SCREEN_WIDTH, 0, SCREEN_WIDTH);
	}

	textmode_move_cursor(0, 0);
}

static void scroll(void)
{
	if (cursor_y >= SCREEN_HEIGHT) {
		uint32_t tmp = (cursor_y - SCREEN_HEIGHT) + 1;

		memcpy(screen_memory, screen_memory + tmp * SCREEN_WIDTH, (SCREEN_HEIGHT - tmp) * SCREEN_WIDTH * 2);
		memsetw(screen_memory + (SCREEN_HEIGHT - tmp) * SCREEN_WIDTH, 0, SCREEN_WIDTH);

		cursor_y = (SCREEN_HEIGHT - 1);

		put_cursor();
	}
}

static void textmode_put(int8_t c)
{
	scroll();

	int8_t *memory = (int8_t *)screen_memory;
	int32_t i = ((cursor_y * SCREEN_WIDTH) + cursor_x) * 2;

	if (c == '\b' && cursor_x) {
		cursor_x--;
	} else if (c == '\t') {
		cursor_x = (cursor_x + 8) & ~(8 - 1);
	} else if (c == '\r') {
		cursor_x = 0;
	} else if (c == '\n') {
		cursor_x = 0;
		cursor_y++;
	} else if (c >= ' ') {
		memory[i] = c;
		i++;
		memory[i] = attrib;
		i++;

		cursor_x++;
	}

	if (cursor_x >= SCREEN_WIDTH) {
		cursor_x = 0;
		cursor_y++;
	}

	put_cursor();
}

static void textmode_write(int8_t *c)
{
	while (*c) {
		textmode_put(*c++);
	}
}

void printf(const int8_t *fmt, ...)
{
	static int8_t buf[1920];

	va_list args;
	int32_t i;

	va_start(args, fmt);
	i = vsprintf(buf, fmt, args);
	va_end(args);

	buf[i] = '\0';

	textmode_write(buf);
}

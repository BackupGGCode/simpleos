#include <main.h>
#include <lib/stdtypes.h>
#include <lib/multiboot.h>
#include <lib/vsprintf.h>
#include <drivers/screen/textmode.h>

int32_t kernel_main(multiboot_t *mboot_ptr)
{
	textmode_init();

	printf("                   _____ _                 _       ____   _____ \n                  / ____(_)               | |     / __ \\ / ____|\n                 | (___  _ _ __ ___  _ __ | | ___| |  | | (___  \n                  \\___ \\| | '_ ` _ \\| '_ \\| |/ _ \\ |  | |\\___ \\ \n                  ____) | | | | | | | |_) | |  __/ |__| |____) |\n                 |_____/|_|_| |_| |_| .__/|_|\\___|\\____/|_____/ \n                                    | |                         \n                                    |_|                         \n");
	textmode_set_colors(COLOR_RED, COLOR_BLACK);
	textmode_move_cursor(19, 6);
	printf("Version %i.%i.%i", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);
	textmode_move_cursor(19, 7);
	printf("Built %s", VERSION_DATE);
	textmode_set_colors(COLOR_GREEN, COLOR_BLACK);
	textmode_move_cursor(41, 6);
	printf("Written by Joe Biellik");
	textmode_set_colors(COLOR_LTGRAY, COLOR_BLACK);
	textmode_move_cursor(0, 9);

	printf("Hello World");

	for (;;);

	return 0x12345678;
}

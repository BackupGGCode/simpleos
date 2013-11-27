#include <main.h>
#include <lib/stdtypes.h>
#include <lib/multiboot.h>
#include <lib/vsprintf.h>
#include <cpu/cpuid.h>
#include <cpu/fpu.h>
#include <interrupt/gdt.h>
#include <interrupt/idt.h>
#include <drivers/pit.h>
#include <drivers/rtc.h>
#include <drivers/serial.h>
#include <drivers/pci/pci.h>
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

	fpu_init();
	gdt_init();
	idt_init();

	__asm __volatile__("sti"); // Start interrupts

	pit_init();
	serial_init();

	// Test serial
	int8_t buffer[50];
	sprintf(buffer, "SimpleOS %i.%i.%i (%s)\n", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_DATE);
	serial_write(SERIAL_PORT_A, buffer);

	// Test RTC
	datetime_t *dt = 0;
	rtc_get(dt);
	printf("Started at %02i-%02i-%02i %02i:%02i:%02i\n\n", dt->year, dt->month, dt->day, dt->hour, dt->min, dt->sec);

	// Test CPU
	cpu_detect();

	printf("\n");

	// Test PCI
	pci_scan();

	for (;;);

	return 0x12345678;
}

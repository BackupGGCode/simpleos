#include <drivers/pit.h>
#include <lib/stdtypes.h>
#include <lib/x86.h>
#include <interrupt/isr.h>
#include <drivers/screen/textmode.h>

volatile uint32_t ticks = 0;

static void pit_callback(registers_t *regs)
{
	ticks++;
}

void pit_init(void)
{
	register_interrupt_handler(IRQ0, &pit_callback);

	int16_t divisor = PIT_FREQUENCY / 1000;

	outportb(0x43, 0x36);
	outportb(0x40, (uint8_t)(divisor & 0xFF));
	outportb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

void pit_wait(uint32_t ms)
{
	uint32_t now = ticks;
	++ms;

	while (ticks - now < ms) {
		;
	}
}

#include <interrupt/isr.h>
#include <lib/stdtypes.h>
#include <lib/x86.h>
#include <drivers/screen/textmode.h>

const int8_t *exception_messages[] = {
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",
	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

interrupt_handler_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, interrupt_handler_t h)
{
	interrupt_handlers[n] = h;
}

void isr_handler(registers_t *regs)
{
	if (interrupt_handlers[regs->int_no]) {
		interrupt_handlers[regs->int_no](regs);
	} else {
		printf("Unhandled interrupt: %i (%s Exception)\n", regs->int_no, exception_messages[regs->int_no]);
		// TODO: Panic
	}
}

void irq_handler(registers_t *regs)
{
	// Send an EOI (end of interrupt) signal to the PICs
	// If this interrupt involved the slave
	if (regs->int_no >= 40) {
		// Send reset signal to slave.
		outportb(0xA0, 0x20);
	}

	// Send reset signal to master. (As well as slave, if necessary)
	outportb(0x20, 0x20);

	if (interrupt_handlers[regs->int_no] != 0) {
		interrupt_handlers[regs->int_no](regs);
	}
}

void irq_ack(int32_t irq)
{
	if (irq >= 12) {
		outportb(0xA0, 0x20);
	}

	outportb(0x20, 0x20);
}

#ifndef DRIVERS_SERIAL_H
#define DRIVERS_SERIAL_H

#include <lib/stdtypes.h>
#include <interrupt/isr.h>

#define SERIAL_IRQ 4

#define SERIAL_PORT_A 0x3F8
#define SERIAL_PORT_B 0x2F8
#define SERIAL_PORT_C 0x3E8
#define SERIAL_PORT_D 0x2E8

void serial_handler_a(registers_t *regs);
void serial_handler_b(registers_t *regs);
void serial_enable(int32_t device);
void serial_init(void);
void serial_write(int32_t device, const int8_t *out);

#endif

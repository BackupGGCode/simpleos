#include <drivers/serial.h>
#include <interrupt/isr.h>
#include <lib/stdtypes.h>
#include <lib/x86.h>
#include <lib/string.h>
#include <drivers/screen/textmode.h>

static int32_t serial_rcvd(int32_t device);
static int8_t serial_recv(int32_t device);
static int8_t serial_recv_async(int32_t device);
static int32_t serial_send_empty(int32_t device);
static void serial_send(int32_t device, int8_t out);

void serial_handler_a(registers_t *regs)
{
	int8_t serial = serial_recv(SERIAL_PORT_A);
	irq_ack(SERIAL_IRQ);

	if (serial == 13) {
		serial = '\n';
	}

	printf("%c", serial);
	serial_send(SERIAL_PORT_B, serial);
}

void serial_handler_b(registers_t *regs)
{
	int8_t serial = serial_recv(SERIAL_PORT_B);
	irq_ack(SERIAL_IRQ - 1);
	serial_send(SERIAL_PORT_A, serial);
}

void serial_enable(int32_t device)
{
	outportb(device + 1, 0x00);
	outportb(device + 3, 0x80); /* Enable divisor mode */
	outportb(device + 0, 0x03); /* Div Low:  03 Set the port to 38400 bps */
	outportb(device + 1, 0x00); /* Div High: 00 */
	outportb(device + 3, 0x03);
	outportb(device + 2, 0xC7);
	outportb(device + 4, 0x0B);
}

void serial_init(void)
{
	serial_enable(SERIAL_PORT_A);
	serial_enable(SERIAL_PORT_B);

	register_interrupt_handler(SERIAL_IRQ, serial_handler_a); /* Install the serial input handler */
	register_interrupt_handler(SERIAL_IRQ - 1, serial_handler_b); /* Install the serial input handler */
	outportb(SERIAL_PORT_A + 1, 0x01);      /* Enable interrupts on receive */
	outportb(SERIAL_PORT_B + 1, 0x01);      /* Enable interrupts on receive */
}

static int32_t serial_rcvd(int32_t device)
{
	return inportb(device + 5) & 1;
}

static int8_t serial_recv(int32_t device)
{
	while (serial_rcvd(device) == 0);

	return inportb(device);
}

static int8_t serial_recv_async(int32_t device)
{
	return inportb(device);
}

static int32_t serial_send_empty(int32_t device)
{
	return inportb(device + 5) & 0x20;
}

static void serial_send(int32_t device, int8_t out)
{
	while (serial_send_empty(device) == 0);

	outportb(device, out);
}

void serial_write(int32_t device, const int8_t *out)
{
	for (int32_t i = 0; i < strlen(out); ++i) {
		serial_send(device, out[i]);
	}
}

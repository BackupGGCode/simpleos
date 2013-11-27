#include <lib/x86.h>
#include <lib/stdtypes.h>

uint8_t inportb(uint16_t port)
{
	uint8_t rv;
	__asm __volatile__("in %%dx,%%al" : "=a"(rv) : "d"(port));
	return rv;
}

uint16_t inportw(uint16_t port)
{
	uint16_t rv;
	__asm __volatile__("in %%dx, %%eax" : "=a"(rv) : "d"(port));
	return rv;
}

uint32_t inportl(uint16_t port)
{
	uint32_t rv;
	__asm __volatile__("in %%dx,%%eax" : "=a"(rv) : "d"(port));
	return rv;
}

void outportb(uint16_t port, uint8_t value)
{
	__asm __volatile__("outb %%al,%%dx" : : "a"(value), "d"(port));
}

void outportw(uint16_t port, uint16_t value)
{
	__asm __volatile__("outw %%ax,%%dx" : : "a"(value), "d"(port));
}

void outportl(uint16_t port, uint32_t value)
{
	__asm __volatile__("outl %%eax,%%dx" : : "a"(value), "d"(port));
}

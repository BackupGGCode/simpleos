#ifndef LIB_IO_H
#define LIB_IO_H

#include <lib/stdtypes.h>

uint8_t inportb(uint16_t port);
uint16_t inportw(uint16_t port);
uint32_t inportl(uint16_t port);
void outportb(uint16_t port, uint8_t value);
void outportw(uint16_t port, uint16_t value);
void outportl(uint16_t port, uint32_t value);

#endif

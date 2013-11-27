#ifndef DRIVERS_PIT_H
#define DRIVERS_PIT_H

#include <lib/stdtypes.h>

#define PIT_FREQUENCY 1193182

void pit_init(void);
void pit_wait(uint32_t ms);

#endif

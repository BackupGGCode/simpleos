#ifndef CPU_FPU_H
#define CPU_FPU_H

#include <lib/stdtypes.h>

void fpu_init(void);
void fpu_set_cw(const uint16_t cw);

#endif

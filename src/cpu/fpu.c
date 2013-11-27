#include <cpu/fpu.h>
#include <lib/stdtypes.h>
#include <drivers/screen/textmode.h>

void fpu_init(void) // TODO: Check FPU is present
{
	size_t cr4;
	__asm __volatile__("mov %%cr4, %0" : "=r"(cr4));
	cr4 |= 0x200;
	__asm __volatile__("mov %0, %%cr4" :: "r"(cr4));

	fpu_set_cw(0x37F);
}

void fpu_set_cw(const uint16_t cw)
{
	__asm __volatile__("fldcw %0" :: "m"(cw));
}

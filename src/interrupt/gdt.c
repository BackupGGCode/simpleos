#include <interrupt/gdt.h>
#include <lib/stdtypes.h>

extern void gdt_flush(uint32_t);

static void gdt_set_gate(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);

gdt_entry_t gdt_entries[3];
gdt_ptr_t gdt_ptr;

void gdt_init(void)
{
	// The limit is the last valid byte from the start of the GDT - i.e. the size of the GDT - 1
	gdt_ptr.limit = sizeof(gdt_entry_t) * 3 - 1;
	gdt_ptr.base = (uint32_t) &gdt_entries;

	gdt_set_gate(0, 0, 0, 0, 0);              // Null segment
	gdt_set_gate(1, 0, 0xFFFFF, 0x9A, 0xCF);  // Code segment
	gdt_set_gate(2, 0, 0xFFFFF, 0x92, 0xCF);  // Data segment

	gdt_flush((uint32_t) &gdt_ptr);
}

static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
	gdt_entries[num].base_low    = (base & 0xFFFF);
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high   = (base >> 24) & 0xFF;

	gdt_entries[num].limit_low   = (limit & 0xFFFF);
	gdt_entries[num].granularity = (limit >> 16) & 0x0F;

	gdt_entries[num].granularity |= gran & 0xF0;
	gdt_entries[num].access      = access;
}

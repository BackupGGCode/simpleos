#include <drivers/pci/driver.h>
#include <lib/stdtypes.h>
#include <lib/x86.h>

uint8_t pci_read8(uint32_t id, uint32_t reg)
{
	uint32_t addr = 0x80000000 | id | (reg & 0xfc);
	outportl(PCI_CONFIG_ADDR, addr);
	return inportb(PCI_CONFIG_DATA + (reg & 0x03));
}

uint16_t pci_read16(uint32_t id, uint32_t reg)
{
	uint32_t addr = 0x80000000 | id | (reg & 0xfc);
	outportl(PCI_CONFIG_ADDR, addr);
	return inportw(PCI_CONFIG_DATA + (reg & 0x02));
}

uint32_t pci_read32(uint32_t id, uint32_t reg)
{
	uint32_t addr = 0x80000000 | id | (reg & 0xfc);
	outportl(PCI_CONFIG_ADDR, addr);
	return inportl(PCI_CONFIG_DATA);
}

void pci_write8(uint32_t id, uint32_t reg, uint8_t data)
{
	uint32_t address = 0x80000000 | id | (reg & 0xfc);
	outportl(PCI_CONFIG_ADDR, address);
	outportb(PCI_CONFIG_DATA + (reg & 0x03), data);
}

void pci_write16(uint32_t id, uint32_t reg, uint16_t data)
{
	uint32_t address = 0x80000000 | id | (reg & 0xfc);
	outportl(PCI_CONFIG_ADDR, address);
	outportw(PCI_CONFIG_DATA + (reg & 0x02), data);
}

void pci_write32(uint32_t id, uint32_t reg, uint32_t data)
{
	uint32_t address = 0x80000000 | id | (reg & 0xfc);
	outportl(PCI_CONFIG_ADDR, address);
	outportl(PCI_CONFIG_DATA, data);
}

static void pci_read_bar(uint32_t id, uint32_t index, uint32_t *address, uint32_t *mask)
{
	uint32_t reg = PCI_CONFIG_BAR0 + index * sizeof(uint32_t);

	// Get address
	*address = pci_read32(id, reg);

	// Find size of the bar
	pci_write32(id, reg, 0xffffffff);
	*mask = pci_read32(id, reg);

	// Restore adddress
	pci_write32(id, reg, *address);
}

void pci_get_bar(pci_bar_t *bar, uint32_t id, uint32_t index)
{
	// Read PCI bar register
	uint32_t addressLow;
	uint32_t maskLow;
	pci_read_bar(id, index, &addressLow, &maskLow);

	if (addressLow & PCI_BAR_64) {
		// 64-bit MMIO
		uint32_t addressHigh;
		uint32_t maskHigh;
		pci_read_bar(id, index + 1, &addressHigh, &maskHigh);

		bar->u.address = (void *)(((uint32_t)addressHigh << 16) | (addressLow & ~0xf));
		bar->size = ~(((uint32_t)maskHigh << 16) | (maskLow & ~0xf)) + 1;
		bar->flags = addressLow & 0xf;
	} else if (addressLow & PCI_BAR_IO) {
		// I/O register
		bar->u.port = (uint16_t)(addressLow & ~0x3);
		bar->size = (uint16_t)(~(maskLow & ~0x3) + 1);
		bar->flags = addressLow & 0x3;
	} else {
		// 32-bit MMIO
		bar->u.address = (void *)(uint32_t)(addressLow & ~0xf);
		bar->size = ~(maskLow & ~0xf) + 1;
		bar->flags = addressLow & 0xf;
	}
}
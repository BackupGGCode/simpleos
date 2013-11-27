#include <drivers/pci/pci.h>
#include <drivers/pci/driver.h>
#include <drivers/pci/registry.h>
#include <lib/stdtypes.h>
#include <lib/string.h>
#include <drivers/screen/textmode.h>

static void pci_check(uint32_t bus, uint32_t dev, uint32_t func)
{
	uint32_t id = PCI_MAKE_ID(bus, dev, func);

	pci_device_info_t info;
	info.vendorId = pci_read16(id, PCI_CONFIG_VENDOR_ID);

	if (info.vendorId == 0xFFFF) {
		return;
	}

	info.deviceId = pci_read16(id, PCI_CONFIG_DEVICE_ID);
	info.progIntf = pci_read8(id, PCI_CONFIG_PROG_INTF);
	info.subclass = pci_read8(id, PCI_CONFIG_SUBCLASS);
	info.classCode = pci_read8(id, PCI_CONFIG_CLASS_CODE);

	uint32_t result = pci_read32(id, PCI_CONFIG_VENDOR_ID);
	uint16_t dev_id = result >> 0x10;
	uint16_t ven_id = result & 0xFFFF;

	printf("%02x:%02x.%d %04x:%04x %s\n", bus, dev, func, info.vendorId, info.deviceId, pci_class_name(info.classCode, info.subclass, info.progIntf));
}

void pci_scan(void)
{
	printf("Scanning PCI bus...\n");

	for (uint32_t bus = 0; bus < 256; ++bus) {
		for (uint32_t dev = 0; dev < 32; ++dev) {
			uint32_t baseId = PCI_MAKE_ID(bus, dev, 0);
			uint8_t headerType = pci_read8(baseId, PCI_CONFIG_HEADER_TYPE);
			uint32_t funcCount = headerType & PCI_TYPE_MULTIFUNC ? 8 : 1;

			for (uint32_t func = 0; func < funcCount; ++func) {
				pci_check(bus, dev, func);
			}
		}
	}
}
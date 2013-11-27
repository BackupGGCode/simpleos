#include <cpu/cpuid.h>
#include <lib/stdtypes.h>
#include <lib/string.h>
#include <drivers/screen/textmode.h>

static inline void cpuid(uint32_t reg, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx)
{
	__asm __volatile__("cpuid" : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx) : "a"(reg));
}

void cpu_detect(void)
{
	printf("Scanning CPU...\n");

	uint32_t eax, ebx, ecx, edx;
	uint32_t largestStandardFunc;

	// Function 0x00 - Vendor-ID and Largest Standard Function

	int8_t vendor[13];

	cpuid(0, &largestStandardFunc, (uint32_t *)(vendor + 0), (uint32_t *)(vendor + 8), (uint32_t *)(vendor + 4));
	vendor[12] = '\0';

	const int8_t *vendorName = "Unknown";

	if (strcmp(vendor, "AuthenticAMD") == 0 || strcmp(vendor, "AMDisbetter") == 0) {
		vendorName = "AMD";
	} else if (strcmp(vendor, "GenuineIntel") == 0) {
		vendorName = "Intel";
	} else if (strcmp(vendor, "CentaurHauls") == 0) {
		vendorName = "Centaur";
	} else if (strcmp(vendor, "CyrixInstead") == 0) {
		vendorName = "Cyrix";
	} else if (strcmp(vendor, "TransmetaCPU") == 0 || strcmp(vendor, "GenuineTMx86") == 0) {
		vendorName = "Transmeta";
	} else if (strcmp(vendor, "Geode by NSC") == 0) {
		vendorName = "National Semiconductor";
	} else if (strcmp(vendor, "NexGenDriven") == 0) {
		vendorName = "NexGen";
	} else if (strcmp(vendor, "RiseRiseRise") == 0) {
		vendorName = "Rise";
	} else if (strcmp(vendor, "SiS SiS SiS ") == 0) {
		vendorName = "SiS";
	} else if (strcmp(vendor, "UMC UMC UMC ") == 0) {
		vendorName = "UMC";
	} else if (strcmp(vendor, "VIA VIA VIA ") == 0) {
		vendorName = "VIA";
	} else if (strcmp(vendor, "Vortex86 SoC") == 0) {
		vendorName = "Vortex";
	} else if (strcmp(vendor, "KVMKVMKVMKVM") == 0) {
		vendorName = "KVM";
	}

	printf("Vendor: %s\n", vendorName);

	// Function 0x01 - Feature Information

	if (largestStandardFunc >= 0x01) {
		cpuid(0x01, &eax, &ebx, &ecx, &edx);

		uint32_t stepping = eax & 0xF;
		uint32_t model = (eax >> 4) & 0xF;
		uint32_t family = (eax >> 8) & 0xF;
		uint32_t exmodel = (eax >> 16) & 0xF;
		uint32_t exfamily = (eax >> 20) & 0xFF;

		uint32_t fullModel = model;
		uint32_t fullFamily = family;

		if (family == 0x06 || family == 0x0F) {
			fullModel = model + (exmodel << 4);
		}

		if (family == 0x0F) {
			fullFamily = family + exfamily;
		}

		printf("Family: %X, Model: %X, Stepping: %X\n", fullFamily, fullModel, stepping);

		printf("Features:");

		if (edx & EDX_PSE) {
			printf(" PSE");
		}

		if (edx & EDX_PAE) {
			printf(" PAE");
		}

		if (edx & EDX_APIC) {
			printf(" APIC");
		}

		if (edx & EDX_MTRR) {
			printf(" MTRR");
		}

		printf("\nInstructions:");

		if (edx & EDX_HTT) {
			printf(" HTT");
		}

		if (edx & EDX_TSC) {
			printf(" TSC");
		}

		if (edx & EDX_MSR) {
			printf(" MSR");
		}

		if (edx & EDX_SSE) {
			printf(" SSE");
		}

		if (edx & EDX_SSE2) {
			printf(" SSE2");
		}

		if (ecx & ECX_SSE3) {
			printf(" SSE3");
		}

		if (ecx & ECX_SSSE3) {
			printf(" SSSE3");
		}

		if (ecx & ECX_SSE41) {
			printf(" SSE41");
		}

		if (ecx & ECX_SSE42) {
			printf(" SSE42");
		}

		if (ecx & ECX_AVX) {
			printf(" AVX");
		}

		if (ecx & ECX_F16C) {
			printf(" F16C");
		}

		if (ecx & ECX_RDRAND) {
			printf(" RDRAND");
		}

		printf("\n");
	}

	// Extended Function 0x00 - Largest Extended Function

	uint32_t largestExtendedFunc;
	cpuid(0x80000000, &largestExtendedFunc, &ebx, &ecx, &edx);

	// Extended Function 0x01 - Extended Feature Bits

	int32_t bits = 32;

	if (largestExtendedFunc >= 0x80000001) {
		cpuid(0x80000001, &eax, &ebx, &ecx, &edx);

		if (edx & EDX_64_BIT) {
			bits = 64;
		}
	}

	printf("Architecture: %i-bit\n", bits);

	// Extended Function 0x02-0x04 - Processor Name / Brand String

	if (largestExtendedFunc >= 0x80000004) {
		int8_t name[48];
		cpuid(0x80000002, (uint32_t *)(name + 0), (uint32_t *)(name + 4), (uint32_t *)(name + 8), (uint32_t *)(name + 12));
		cpuid(0x80000003, (uint32_t *)(name + 16), (uint32_t *)(name + 20), (uint32_t *)(name + 24), (uint32_t *)(name + 28));
		cpuid(0x80000004, (uint32_t *)(name + 32), (uint32_t *)(name + 36), (uint32_t *)(name + 40), (uint32_t *)(name + 44));

		const int8_t *p = name;

		while (*p == ' ') {
			++p;
		}

		printf("CPU Name: %s\n", p);
	}
}

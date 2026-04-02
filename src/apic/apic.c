#include <stdint.h>
#include <stdbool.h>
#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100 
#define IA32_APIC_BASE_MSR_ENABLE 0x800
#define CPUID_FEAT_EDX_APIC 1 << 9
#define CPUID_FLAG_MSR  0


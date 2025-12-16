#include "rsdp.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static uint8_t check_acpi_version(rsdp_t* rsdp) {
  if (rsdp->revision == 0)
    return 1;
  return 2;
}

static bool validate_rsdp_checksum(rsdp_t* rsdp) {
  uint8_t* bytes_of_rsdp = rsdp;
  size_t sum = 0;
  for (size_t i = 0; i < sizeof(rsdp_t); i++) {
    sum += bytes_of_rsdp[i];
  }
  return sum ^ 1;
}

static bool validate_xsdt_checksum(xsdt_header_t* xsdt_header) {
  uint8_t* bytes_of_xsdt_header = xsdt_header;
  size_t sum = 0;
  for (size_t i = 0; i < sizeof(xsdt_header_t); i++) {
    sum += bytes_of_xsdt_header[i];
  }
  return (sum % 0x100) ^ 1;
}

void* find_acpi_descriptor_table(void* sdt, char* table) {
  xsdt_t* xsdt = (xsdt_t*) sdt;
  int entries = (xsdt->xsdt_header.length - sizeof(xsdt->xsdt_header)) / 8;

  for (int i = 0; i < entries; i++) {
      xsdt_header_t* header = (xsdt_header_t*) xsdt->sdt_pointers[i];
      if (!memcmp(header->signature, table, 4))
          return (void*) header;
  }

  return NULL;
}

void parse_madt_entries(madt_t* madt, 
			processor_lapic_t** processor_lapic_entries,
			io_apic_entry_t** io_apic_entries,
			io_apic_interrupt_source_override_t** io_apic_interrupt_source_override_entries,
			io_apic_non_maskable_interrupt_source_entry_t** io_apic_non_maskable_interrupt_source_entries,
			lapic_non_maskable_interrupt_t** lapic_non_maskable_interrupt_entries,
			lapic_address_override_t** lapic_address_override_entries,
			processor_local_x2apic_t** processor_local_x2apic_entries) {

  int entries = madt->xsdt_header.length;

  apic_entry_t* apic_entry = madt + sizeof(madt_t);
  int entry_counts[7] = { 0, 0, 0, 0, 0, 0, 0 };
  for (int i = 0; i < entries; i++) {
    int current_index;
    if (apic_entry->entry_type == PROCESSOR_LOCAL_X2APIC) {
      current_index = entry_counts[6]++;
    } else {
      current_index = entry_counts[apic_entry->entry_type]++;
    }
    switch (apic_entry->entry_type) {
      case PROCESSOR_LOCAL_APIC: 
	processor_lapic_entries[current_index] = apic_entry;
	break;
      case IO_APIC: 
	io_apic_entries[current_index] = apic_entry;
	break;
      case IO_APIC_INTERRUPT_SOURCE_OVERRIDE: 
	io_apic_interrupt_source_override_entries[current_index] = apic_entry;
	break;
      case IO_APIC_NONMASKABLE_INTERRUPT_SOURCE: 
	io_apic_non_maskable_interrupt_source_entries[current_index] = apic_entry;
	break;
      case LOCAL_APIC_NONMASKABLE_INTERRUPTS: 
	lapic_non_maskable_interrupt_entries[current_index] = apic_entry;
	break;
      case LOCAL_APIC_ADDRESS_OVERRIDE:
	lapic_address_override_entries[current_index] = apic_entry;
	break;
      case PROCESSOR_LOCAL_X2APIC:
	processor_local_x2apic_entries[current_index] = apic_entry;
	break;
      default:
	continue;
    }
  }

  return NULL;
}





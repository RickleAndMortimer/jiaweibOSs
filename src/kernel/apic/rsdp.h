#include <stdint.h>

typedef struct rsdp_t {
  char signature[8];
  uint8_t checksum;
  char oem_id[6];
  uint8_t revision;
  uint32_t rsdt_address;
} rsdp_t __attribute__ ((packed));

typedef struct xsdp_t {
  rsdp_t rsdp;
  uint32_t length;
  uint64_t xsdt_address;
  uint8_t extended_checksum;
  uint8_t reserved[3];
} xsdp_t __attribute__ ((packed));

typedef struct xsdt_header_t {
  char signature[4];
  uint32_t length;
  uint8_t revision;
  uint8_t checksum;
  char oem_id[6];
  char oem_table_id[8];
  uint32_t oem_revision;
  uint32_t creator_id;
  uint32_t creator_revision;
} xsdt_header_t __attribute__ ((packed));

typedef struct xsdt_t {
  xsdt_header_t xsdt_header;
  uint64_t* sdt_pointers;
} xsdt_t;

typedef struct madt_t {
  xsdt_header_t xsdt_header;
  uint32_t lapic_address;
  uint32_t flags;
  uint32_t reserved;
} madt_t;

typedef enum APIC_ENTRY_TYPE {
  PROCESSOR_LOCAL_APIC = 0,
  IO_APIC = 1,
  IO_APIC_INTERRUPT_SOURCE_OVERRIDE = 2,
  IO_APIC_NONMASKABLE_INTERRUPT_SOURCE = 3,
  LOCAL_APIC_NONMASKABLE_INTERRUPTS = 4,
  LOCAL_APIC_ADDRESS_OVERRIDE = 5,
  PROCESSOR_LOCAL_X2APIC = 9
} APIC_ENTRY_TYPE;

typedef struct apic_entry_t {
  uint8_t entry_type;
  uint8_t record_length;
} apic_entry_t;

typedef struct processor_lapic_entry_t {
  apic_entry_t apic_entry;
  uint8_t acpi_processor_id;
  uint8_t apic_id;
  uint32_t flags;
} processor_lapic_t;

typedef struct io_apic_entry_t {
  apic_entry_t apic_entry;
  uint8_t io_apic_id;
  uint8_t reserved;
  uint32_t io_apic_address;
  uint32_t global_system_interrupt_base;
} io_apic_entry_t;

typedef struct io_apic_interrupt_source_override_t {
  apic_entry_t apic_entry;
  uint8_t acpi_processor_id;
  uint16_t flags;
  uint8_t lint_num;
} io_apic_interrupt_source_override_t;

typedef struct io_apic_non_maskable_interrupt_source_entry_t {
  apic_entry_t apic_entry;
  uint8_t nmi_source;
  uint8_t reserved;
  uint16_t flags;
  uint32_t global_system_interrupt;
} io_apic_non_maskable_interrupt_source_entry_t;

typedef struct lapic_non_maskable_interrupt_t {
  apic_entry_t apic_entry;
  uint8_t acpi_processor_id;
  uint16_t flags;
  uint8_t lint_num;
} lapic_non_maskable_interrupt_t;

typedef struct lapic_address_override_t {
  apic_entry_t apic_entry;
  uint16_t reserved;
  uint64_t physical_address;
} lapic_address_override_t;

typedef struct processor_local_x2apic_t {
  apic_entry_t apic_entry;
  uint16_t reserved;
  uint32_t x2apic_id;
  uint32_t flags;
  uint32_t acpi_id;
} processor_local_x2apic_t;

void* find_acpi_descriptor_table(void* sdt, char* table);
void parse_madt_entries(madt_t* madt, 
			processor_lapic_t** processor_lapic_entries,
			io_apic_entry_t** io_apic_entries,
			io_apic_interrupt_source_override_t** io_apic_interrupt_source_override_entries,
			io_apic_non_maskable_interrupt_source_entry_t** io_apic_non_maskable_interrupt_source_entries,
			lapic_non_maskable_interrupt_t** lapic_non_maskable_interrupt_entries,
			lapic_address_override_t** lapic_address_override_entries,
			processor_local_x2apic_t** processor_local_x2apic_entries);

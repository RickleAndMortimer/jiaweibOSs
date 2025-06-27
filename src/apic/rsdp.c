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

static void* get_xsdt(xsdt_t* xsdt, char* signature) {
  int entries = (xsdt->xsdt_header.length - sizeof(xsdt_header_t)) / 8;
  for (size_t i = 0; i < entries; i++) {
    xsdt_header_t* xsdt_header = (xsdt_header_t*) xsdt->sdt_pointers[i];
    if (!strncmp(xsdt_header->signature, signature, 4))
      return xsdt_header;
  }
  return NULL;
}


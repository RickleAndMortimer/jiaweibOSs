#include <stdint.h>
#include "pci/pci.h"

uint32_t nvme_read_reg(uint64_t nvme_base_addr, uint32_t offset) {
  volatile uint32_t *nvme_reg = (volatile uint32_t *)(nvme_base_addr + offset);
  map_page((uint64_t)nvme_reg);
  return *nvme_reg;
}


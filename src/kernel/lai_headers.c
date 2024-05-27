#include "lai_headers.h"

void* laihost_malloc(size_t bytes) {
    malloc(bytes);
}

void laihost_free(void* address) {
    free(address);
}

// TODO replace this with an actual realloc implementation
void* laihost_realloc(void* address, size_t length) {
    return address;
}

// TODO implement
void laihost_log(int level, const char *msg) {

}

// TODO implement
__attribute__((noreturn)) void laihost_panic(const char *msg) {

}


// TODO implement
void laihost_outb(uint16_t port, uint8_t val);
void laihost_outw(uint16_t port, uint16_t val);
void laihost_outd(uint16_t port, uint32_t val);

// TODO implement
uint8_t laihost_inb(uint16_t port);
uint16_t laihost_inw(uint16_t port);
uint32_t laihost_ind(uint16_t port);

// TODO implement
uint8_t laihost_pci_readb(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset);
uint16_t laihost_pci_readw(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset);
uint32_t laihost_pci_readd(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset);

// TODO implement
void laihost_pci_writeb(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset, uint8_t val);
void laihost_pci_writew(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset, uint16_t val);
void laihost_pci_writed(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset, uint32_t val);

// TODO implement
void laihost_sleep(uint64_t ms);

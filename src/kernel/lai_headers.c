#include "lai_headers.h"
#include "mem/paging.h"
#include "mem/heap.h"
#include <printf/printf.h>
#include <sys/io.h>
#include <sys/halt.h>

void* laihost_malloc(size_t bytes) {
    return malloc(bytes);
}

void laihost_free(void* address) {
    free(address);
}

// TODO replace this with an actual realloc implementation
void* laihost_realloc(void* address, size_t length) {
    realloc(address, length);
}

// TODO: Implement
void* laihost_map(size_t address, size_t count) {
}

void laihost_unmap(void* pointer, size_t count) {
  unmap_page(pointer, pointer, count);
}

void laihost_log(int level, const char *msg) {
    printf("[LAI]: Level %d, %s", level, msg);
}

__attribute__((noreturn)) void laihost_panic(const char *msg) {
    printf("[LAI]: Kernel Panic! %s", msg);
    hcf();
}

void laihost_outb(uint16_t port, uint8_t val) {
    outb(port, val);
}

void laihost_outw(uint16_t port, uint16_t val) {
    outw(port, val);
}

void laihost_outd(uint16_t port, uint32_t val) {
    outl(port, val);
}

uint8_t laihost_inb(uint16_t port) {
    inb(port);
}

uint16_t laihost_inw(uint16_t port) {
    inw(port);
}

uint32_t laihost_ind(uint16_t port) {
    inl(port);
}


void* laihost_scan(char* sig, size_t index);

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

#include "io.h"

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

void outb(uint16_t port, uint8_t value) {
    asm volatile ( "outb %0, %1" : : "a"(value), "d"(port) : "memory");
}

void io_wait(void) {
    outb(0x80, 0);
}

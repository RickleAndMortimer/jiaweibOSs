#ifndef IO_H
#define IO_H

#include <stdint.h>
#include <stddef.h>

inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

inline uint8_t inb_p(uint16_t port); 

inline uint16_t inw(uint16_t port);
inline uint16_t inw_p(uint16_t port);
inline uint32_t inl(uint16_t port);
inline uint32_t inl_p(uint16_t port);

inline void outb(uint8_t value, uint16_t port) {
    asm volatile ( "outb %0, %1" : : "a"(value), "Nd"(port) : "memory");
}

inline void outb_p(uint8_t value, uint16_t port);
inline void outw(uint16_t value, uint16_t port);
inline void outw_p(uint16_t value, uint16_t port);
inline void outl(uint32_t value, uint16_t port);
inline void outl_p(uint32_t value, uint16_t port);

inline void insb(uint16_t port, void* addr, size_t count);
inline void insw(uint16_t port, void* addr, size_t count);
inline void insl(uint16_t port, void* addr, size_t count);
inline void outsb(uint16_t port, const void* addr, size_t count);
inline void outsw(uint16_t port, const void* addr, size_t count);
inline void outsl(uint16_t port, const void* addr, size_t count);

inline void io_wait(void)
{
    outb(0x80, 0);
}

#endif

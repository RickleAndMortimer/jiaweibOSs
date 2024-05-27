#ifndef IO_H
#define IO_H

#include <stdint.h>
#include <stddef.h>

uint8_t inb(uint16_t port); 
uint8_t inb_p(uint16_t port); 

uint16_t inw(uint16_t port);
uint16_t inw_p(uint16_t port);
uint32_t inl(uint16_t port);
uint32_t inl_p(uint16_t port);

void outb(uint16_t port, uint8_t value);

void outb_p(uint8_t value, uint16_t port);
void outw(uint16_t value, uint16_t port);
void outw_p(uint16_t value, uint16_t port);
void outl(uint32_t value, uint16_t port);
void outl_p(uint32_t value, uint16_t port);

void insb(uint16_t port, void* addr, size_t count);
void insw(uint16_t port, void* addr, size_t count);
void insl(uint16_t port, void* addr, size_t count);
void outsb(uint16_t port, const void* addr, size_t count);
void outsw(uint16_t port, const void* addr, size_t count);
void outsl(uint16_t port, const void* addr, size_t count);

void io_wait(void);

#endif

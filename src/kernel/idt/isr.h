#ifndef ISR_H
#define ISR_H

#include <stdint.h>

extern "C" void exception_handler(uint64_t error_code, uint64_t rip, uint64_t cs, uint64_t rflags, uint64_t rsp);

#endif

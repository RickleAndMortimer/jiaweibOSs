#ifndef ISR_H
#define ISR_H

#include <stdint.h>

typedef struct interrupt_frame_t
{
    uint64_t r11, r10, r9, r8;
    uint64_t rsi, rdi, rdx, rcx, rax;
    uint64_t int_no, err_code;
    uint64_t rsp, rflags, cs, rip;
}__attribute__((packed)) interrupt_frame_t;

void exception_handler(interrupt_frame_t* frame);
void irq_handler(interrupt_frame_t* frame);

#endif

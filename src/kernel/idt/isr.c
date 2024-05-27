#include "isr.h"
#include "../gfx/term.h"

void (*interrupt_handlers[256]) (interrupt_frame_t* frame);

void exception_handler(interrupt_frame_t* frame) {
	kprint("Handling an exception\n", 23);
}

void irq_handler(interrupt_frame_t* frame) {
	kprint("Handling an IRQ call\n", 24);
	interrupt_handlers[frame->int_no](frame);
}

void register_irq_handler(uint8_t interrupt, void (*handler) (interrupt_frame_t* frame)) {
	interrupt_handlers[interrupt] = handler;
}

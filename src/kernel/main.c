#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "limine.h"

#include "idt/idt.h"
#include "drivers/pit.h"
#include "drivers/pic.h"
#include "gfx/term.h"
#include "mem/linked_list_allocator.h"

#include <flanterm/flanterm.h>
#include <flanterm/backends/fb.h>

__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2);

__attribute__((used, section(".requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

__attribute__((used, section(".requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 1
};

__attribute__((used, section(".requests")))
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 1
};

__attribute__((used, section(".requests")))
static volatile struct limine_smp_request smp_request = {
    .id = LIMINE_SMP_REQUEST,
    .revision = 1
};

__attribute__((used, section(".requests")))
static volatile struct limine_stack_size_request stack_size_request = {
    .id = LIMINE_STACK_SIZE_REQUEST,
    .revision = 1,
    .stack_size = 0x100000
};

__attribute__((used, section(".requests_start_marker")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
static volatile LIMINE_REQUESTS_END_MARKER;

// Halt and catch fire function.
static void cpu2_test(void) {
    struct limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];
    // struct flanterm_context *ft_ctx = initialize_terminal(framebuffer); init_idt(); enable_interrupts();

    for (size_t i = 0; i < 100; i++) {
        volatile uint32_t* fb_ptr = framebuffer->address;
        fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xff0000;
    }

    const char msg[] = "Hello from cpu2!\n";
    // ftprint(ft_ctx, msg, sizeof(msg));

    for (;;) {
        asm ("hlt;");
    }
}


// Halt and catch fire function.
static void hcf(void) {
    for (;;) {
        asm ("hlt;");
    }
}

void _start(void) {
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // Ensure we got a memmap.
    if (memmap_request.response == NULL) {
        hcf();
    }


    // Access SMP information.
    if (smp_request.response == NULL) {
        hcf();
    }

    // Fetch the first framebuffer.

    struct limine_memmap_entry** entries = memmap_request.response->entries;
    struct limine_memmap_entry* used_entry;

    for (size_t i = 0; i < memmap_request.response->entry_count; i++) {
	if (entries[i]->type == LIMINE_MEMMAP_USABLE) {
	    used_entry = entries[i];
	    break;
	}
    }

    uint64_t offset = hhdm_request.response->offset;

    linked_list_allocator_t* allocator = (linked_list_allocator_t*) offset + used_entry->base;
    size_t free_list[30];
    initialize_linked_list_allocator(allocator, 30, allocator + 1, 32, free_list);

    allocator->allocator.malloc((physical_memory_allocator_t*) allocator, 24);
    char* g = allocator->allocator.malloc((physical_memory_allocator_t*) allocator, 28);
    g[0] = 'g';
    allocator->allocator.malloc((physical_memory_allocator_t*) allocator, 28);

    allocator->allocator.free((physical_memory_allocator_t*) allocator, g);

    struct limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];
    
    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    for (size_t i = 0; i < 100; i++) {
        volatile uint32_t* fb_ptr = framebuffer->address;
        fb_ptr[500 + i * (framebuffer->pitch / 4) + i] = 0xff0000;
    }

    // struct flanterm_context *ft_ctx = initialize_terminal(framebuffer);
    
    struct limine_smp_response *cpus = smp_request.response;
    cpus->cpus[1]->goto_address = cpu2_test;

    // remap_pic(0x20, 0x28);
    // clear_mask_for_irq(0);
    init_idt();
    enable_interrupts();
    // init_pit(50000);

    asm volatile ("int $10;");
    asm volatile ("int $11;");
    asm volatile ("int $12;");

    const char msg[] = "Hello world\n";
    // ftprint(ft_ctx, msg, sizeof(msg));

    // We're done, just hang...
    hcf();
}

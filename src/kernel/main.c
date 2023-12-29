#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "limine.h"
#include "idt/idt.h"
#include "terminal/terminal.h"
#include "mem/linked_list_allocator.h"

#include <flanterm/flanterm.h>
#include <flanterm/backends/fb.h>

// Set the base revision to 1, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.
LIMINE_BASE_REVISION(1)

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, in C, they should
// NOT be made "static".

struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 1
};

struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 1
};

// Halt and catch fire function.
static void hcf(void) {
    for (;;) {
        asm ("hlt;");
    }
}

// The following will be our kernel's entry point.
// If renaming _start() to something else, make sure to change the
// linker script accordingly.
void _start(void) {
    // Ensure the bootloader actually understands our base revision (see spec).
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
    initialize_linked_list_allocator(allocator, 4096, allocator + 1, 32);

    char* c = allocator->allocator.malloc((physical_memory_allocator_t*) allocator, 24);
    char* g = allocator->allocator.malloc((physical_memory_allocator_t*) allocator, 28);

    c[0] = 'h';
    g[1] = 'k';

    struct limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];
    
    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    for (size_t i = 0; i < 100; i++) {
        volatile uint32_t* fb_ptr = framebuffer->address;
        fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xff0000;
    }

    struct flanterm_context *ft_ctx = flanterm_fb_simple_init(
	framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch
    );

    init_idt();
    enable_interrupts();

    asm volatile ("int $10;");
    asm volatile ("int $11;");
    asm volatile ("int $12;");

    const char msg[] = "Hello world\n";
    flanterm_write(ft_ctx, msg, sizeof(msg));

    // We're done, just hang...
    hcf();
}

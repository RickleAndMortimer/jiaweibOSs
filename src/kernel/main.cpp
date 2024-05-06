#include <cstdint>
#include <cstddef>
#include <cstdbool>

#include "limine.h"
/*
#include "idt/idt.h"
#include "mem/linked_list_allocator.h"
*/

#include <flanterm/flanterm.h>    
#include <flanterm/backends/fb.h>

namespace {
    __attribute__((used, section(".requests")))
    volatile LIMINE_BASE_REVISION(2);
}

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, in C, they should
// NOT be made "static".

namespace {
    __attribute__((used, section(".requests")))
    volatile limine_framebuffer_request framebuffer_request = {
        .id = LIMINE_FRAMEBUFFER_REQUEST,
        .revision = 0,
	.response = nullptr
    };


    __attribute__((used, section(".requests")))
    volatile limine_memmap_request memmap_request = {
        .id = LIMINE_MEMMAP_REQUEST,
        .revision = 1,
	.response = nullptr
    };

    __attribute__((used, section(".requests")))
    volatile limine_hhdm_request hhdm_request = {
        .id = LIMINE_HHDM_REQUEST,
        .revision = 1,
	.response = nullptr
    };
}

namespace {
    // Halt and catch fire function.
    static void hcf(void) {
        for (;;) {
            asm ("hlt;");
        }
    }
}

namespace {
    __attribute__((used, section(".requests_start_marker")))
    volatile LIMINE_REQUESTS_START_MARKER;

    __attribute__((used, section(".requests_end_marker")))
    volatile LIMINE_REQUESTS_END_MARKER;
}

extern "C" {
    int __cxa_atexit(void (*)(void *), void *, void *) { return 0; }
    void __cxa_pure_virtual() { hcf(); }
}

extern void (*__init_array[])();
extern void (*__init_array_end[])();

// The following will be our kernel's entry point.
// If renaming _start() to something else, make sure to change the
// linker script accordingly.

extern "C" {
    void _start(void) {
        // Ensure the bootloader actually understands our base revision (see spec).
        if (LIMINE_BASE_REVISION_SUPPORTED == false) {
            hcf();
        }

	for (std::size_t i = 0; &__init_array[i] != __init_array_end; i++) {
            __init_array[i]();
	}
    
        // Ensure we got a framebuffer.
        if (framebuffer_request.response == nullptr || framebuffer_request.response->framebuffer_count < 1) {
            hcf();
        }
    
        // Ensure we got a memmap.
        if (memmap_request.response == nullptr) {
            hcf();
        }
    
        limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];
        
	/*
        // Note: we assume the framebuffer model is RGB with 32-bit pixels.
        for (size_t i = 0; i < 100; i++) {
            volatile std::uint32_t* fb_ptr = static_cast<volatile std::uint32_t*>(framebuffer->address);
            fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xff0000;
        }
	*/

	flanterm_context *ft_ctx = flanterm_fb_init(
		nullptr,
		nullptr,
		reinterpret_cast<std::uint32_t*>(framebuffer->address), framebuffer->width, framebuffer->height, framebuffer->pitch,
		framebuffer->red_mask_size, framebuffer->red_mask_shift,
		framebuffer->green_mask_size, framebuffer->green_mask_shift,
		framebuffer->blue_mask_size, framebuffer->blue_mask_shift,
		nullptr,
		nullptr, nullptr,
		nullptr, nullptr,
		nullptr, nullptr,
		nullptr, 0, 0, 1,
		0, 0,
		0
    	);
    
	/*
        init_idt();
        enable_interrupts();
    
        asm volatile ("int $10;");
        asm volatile ("int $11;");
        asm volatile ("int $12;");
	*/
    
        const char msg[] = "Hello world\n";
        flanterm_write(ft_ctx, msg, sizeof(msg));
    
        // We're done, just hang...
        hcf();
    }
}

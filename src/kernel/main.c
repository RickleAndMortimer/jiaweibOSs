#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "limine.h"

#include "idt/idt.h"
#include "drivers/pit.h"
#include "drivers/pic.h"
#include "gfx/term.h"
#include "sys/halt.h"
#include "mem/linked_list_allocator.h"
#include "mem/page_heap.h"
#include "mem/heap.h"
#include "mem/buddy_allocator.h"

#include <flanterm/flanterm.h>
#include <flanterm/backends/fb.h>
#include <lai/helpers/pm.h>
#include <printf/printf.h>

typedef struct rsdp_t {
 char signature[8];
 uint8_t Checksum;
 char oemid[6];
 uint8_t revision;
 uint32_t rsdt_address;
}__attribute__ ((packed)) rsdp_t;

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

__attribute__((used, section(".requests")))
static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0
};

__attribute__((used, section(".requests_start_marker")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
static volatile LIMINE_REQUESTS_END_MARKER;

// Halt and catch fire function.
static void cpu2_test(void) {
    struct limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];
    // struct flanterm_context *ft_ctx = initialize_terminal(framebuffer); 
    // init_idt(); 
    // enable_interrupts();

    for (size_t i = 0; i < 100; i++) {
        volatile uint32_t* fb_ptr = framebuffer->address;
        fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xff0000;
    }

    const char msg[] = "Hello from cpu2!\n";
    // ftprint(ft_ctx, msg, sizeof(msg));

    hcf();
}


static void init_interrupts() {
    remap_pic(0x20, 0x28);
    clear_mask_for_irq(0);
    init_idt();
    enable_interrupts();
    init_pit(50000);
}

static void test_interrupts() {
    asm volatile ("int $10;");
    asm volatile ("int $11;");
    asm volatile ("int $12;");
}

static void setup_lai() {
    // Access RSDP information.
    if (rsdp_request.response == NULL) {
        hcf();
    }

    // Get rsdp address
    rsdp_t* rsdp = rsdp_request.response->address;

    lai_set_acpi_revision(rsdp->revision);
    lai_create_namespace();
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

    struct limine_memmap_entry** entries = memmap_request.response->entries;
    struct limine_memmap_entry* used_entry;

    for (size_t i = 0; i < memmap_request.response->entry_count; i++) {
	    if (entries[i]->type == LIMINE_MEMMAP_USABLE) {
		used_entry = entries[i];
		break;
	    }
    }

    /*
    initialize_default_request_heap_sizes(offset + used_entry->base);
    volatile char* x = linked_list_heap_fast_malloc(24);
    volatile char* z = linked_list_heap_fast_malloc(32);
    volatile char* y = linked_list_heap_fast_malloc(48);
    strcpy(x, "hello");
    strcpy(z, "jello");
    strcpy(y, "goodbye");
    linked_list_free(x);
    x = linked_list_heap_fast_malloc(24);
    strcpy(x, "bello");
    */

    // Fetch the first framebuffer.
    struct limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];
    
    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    for (size_t i = 0; i < 100; i++) {
        volatile uint32_t* fb_ptr = framebuffer->address;
        fb_ptr[500 + i * (framebuffer->pitch / 4) + i] = 0xff0000;
    }

    struct flanterm_context *ft_ctx = initialize_terminal(framebuffer);
    
    struct limine_smp_response *cpus = smp_request.response;
    // cpus->cpus[1]->goto_address = cpu2_test;

    const char msg[] = "Hello world\n";
    ftprint(ft_ctx, msg, sizeof(msg));
    printf("ayo %d", 50);

    printf("%d %d %d\n", used_entry->base, used_entry->length, used_entry->type);

    uint64_t offset = hhdm_request.response->offset;
    buddy_allocator_t* alloc = offset + used_entry->base;
    buddy_free_node_t* free_list = (char*) alloc + sizeof(buddy_allocator_t);
    size_t capacity = 4096;
    size_t min_block_size = 64;
    size_t free_list_length = (capacity / min_block_size - 1);
    buddy_init((buddy_allocator_t*) alloc, free_list, capacity, min_block_size, free_list + free_list_length);

    void* addd = buddy_alloc(alloc, 1024);
    void* add = buddy_alloc(alloc, 2048);
    void* ad = buddy_alloc(alloc, 1024);
    printf("%p %p %p \n", addd, add, ad);
    buddy_free(alloc, addd);
    buddy_free(alloc, add);
    buddy_free(alloc, ad);
    // We're done, just hang...
    hcf();
}

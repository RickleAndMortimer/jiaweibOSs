#include <stdint.h>
#include <stddef.h>
#include "paging.h"

static inline void flush_tlb(void* page) {
    __asm__ volatile ("invlpg (%0)" :: "r" (page) : "memory");
}

uint64_t read_cr3(void) {
    uint64_t val;
    __asm__ volatile ("mov %%cr3, %0" : "=r"(val));
    return val;
}

page_table_t* get_pml4() {
    uintptr_t cr3 = (uintptr_t) read_cr3();
    pml4 = (page_table_t*) ((cr3 >> 12) << 12);

    return pml4;
}

void set_cr3(size_t page_table_addr) {
    __asm__ volatile ("mov %0, %%cr3" : "=r"(page_table_addr));
}

void set_page_table_entry(page_entry_t* entry, uint8_t flags, uintptr_t physical_address, uint16_t available) {
    entry->present = (flags >> 1) & 1;
    entry->writable = (flags >> 2) & 1;
    entry->user_accessible = (flags >> 3) & 1;
    entry->write_through_caching = (flags & 0x8) & 1;
    entry->disable_cache = (flags & 0x10) & 1;
    entry->null = (flags & 0x20) & 1;
    entry->global = (flags & 0x40) & 1;
    entry->avl1 = available & 0x3;
    entry->physical_address = physical_address; 
    entry->avl2 = available >> 3;
    entry->no_execute = flags >> 7;
}

void* get_phys_addr(void* virtual_addr) {
    uintptr_t address = (uintptr_t) virtual_addr;

    uint64_t offset = address & 0xFFF;
    uint64_t page_table_index = (address >> 12) & 0x1FF;
    uint64_t page_directory_index = (address >> 21) & 0x1FF;
    uint64_t page_directory_pointer_index = (address >> 30) & 0x1FF;
    uint64_t pml4_index = (address >> 39) & 0x1FF;

    page_table_t* page_directory_pointer = (page_table_t*) ((uint64_t) (pml4->entries[pml4_index].physical_address) << 12);
    page_table_t* page_directory = (page_table_t*) ((uint64_t) (page_directory_pointer->entries[page_directory_pointer_index].physical_address) << 12);
    page_table_t* page_table = (page_table_t*) ((uint64_t) (page_directory->entries[page_directory_index].physical_address) << 12);

    return (void*) ((page_table->entries[page_table_index].physical_address << 12) + offset);
}

static void allocate_entry(page_table_t* table, size_t index, uint8_t flags) {
    void* physical_address = malloc(4096);
    set_page_table_entry(&(table->entries[index]), flags, (uintptr_t) physical_address >> 12, 0);
}

void map_page(void* virtual_address, void* physical_address, uint8_t flags) {
    // Make sure that both addresses are page-aligned.
    uintptr_t virtual_address_int = (uintptr_t) virtual_address;
    uintptr_t physical_address_int = (uintptr_t) physical_address;

    uint64_t pml4_index = (virtual_address_int >> 39) & 0x1FF;
    uint64_t page_directory_pointer_index = (virtual_address_int >> 30) & 0x1FF;
    uint64_t page_directory_index = (virtual_address_int >> 21) & 0x1FF;
    uint64_t page_table_index = (virtual_address_int >> 12) & 0x1FF;
 
    if (!pml4->entries[pml4_index].present)
        allocate_entry(pml4, pml4_index, flags);

    page_table_t* page_directory_pointer = (page_table_t*) (uint64_t) (pml4->entries[pml4_index].physical_address << 12);

    if (!page_directory_pointer->entries[page_directory_pointer_index].present) 
        allocate_entry(page_directory_pointer, page_directory_pointer_index, flags);

    page_table_t* page_directory = (page_table_t*) (uint64_t) (page_directory_pointer->entries[page_directory_pointer_index].physical_address << 12);

    if (!page_directory->entries[page_directory_index].present) 
        allocate_entry(page_directory, page_directory_index, flags);
    
    page_table_t* page_table = (page_table_t*) (uint64_t) (page_directory->entries[page_directory_index].physical_address << 12);

    // Now you need to flush the entry in the TLB
    flush_tlb(virtual_address);
}

void unmap_page(void* virtual_address, void* physical_address, uint8_t flags) {
    uintptr_t virtual_address_int = (uintptr_t) virtual_address;
    uintptr_t physical_address_int = (uintptr_t) physical_address;

    uint64_t pml4_index = (virtual_address_int >> 39) & 0x1FF;
    uint64_t page_directory_pointer_index = (virtual_address_int >> 30) & 0x1FF;
    uint64_t page_directory_index = (virtual_address_int >> 21) & 0x1FF;
    uint64_t page_table_index = (virtual_address_int >> 12) & 0x1FF;
 
    if (!pml4->entries[pml4_index].present)
        allocate_entry(pml4, pml4_index, flags);

    page_table_t* page_directory_pointer = (page_table_t*) (uint64_t) (pml4->entries[pml4_index].physical_address << 12);

    if (!page_directory_pointer->entries[page_directory_pointer_index].present) 
        allocate_entry(page_directory_pointer, page_directory_pointer_index, flags);

    page_table_t* page_directory = (page_table_t*) (uint64_t) (page_directory_pointer->entries[page_directory_pointer_index].physical_address << 12);

    if (!page_directory->entries[page_directory_index].present) 
        allocate_entry(page_directory, page_directory_index, flags);
    
    page_table_t* page_table = NULL;

    flush_tlb(virtual_address);
}

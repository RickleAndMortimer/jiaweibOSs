#ifndef PAGING_H
#define PAGING_H
#include <stdint.h>

// options1:     R P/W U/S PWT PCD A D PS(1) G AVL
// Bits (0-11):  1 1   1   1   1   1 1 1     1 3   
   
// bits (12-51): address (significant bits are 0 by default,)

// options2:        AVL PK XD
// Bits (52-63):    7   4  1 

typedef struct page_entry_t {
    uint8_t present : 1;
    uint8_t writable : 1;
    uint8_t user_accessible : 1;
    uint8_t write_through_caching : 1;
    uint8_t disable_cache : 1;
    uint8_t accessed : 1;
    uint8_t dirty : 1;
    uint8_t null : 1;
    uint8_t global : 1;
    uint8_t avl1 : 3;
    uintptr_t physical_address : 40;
    uint16_t avl2 : 11;
    uint8_t no_execute : 1;
} page_entry_t;

typedef struct page_table_t {
    page_entry_t entries[512];
} page_table_t;

static page_table_t* pml4;

page_table_t* init_pml4(void); 
void* get_phys_addr(void* virtual_addr);
void map_page(void* virtual_address, void* physical_address, uint8_t flags);
void unmap_page(void* virtual_address, void* physical_address, uint8_t flags);
void set_page_table_entry(page_entry_t* entry, uint8_t flags, uintptr_t physical_address, uint16_t available);
void set_cr3(size_t page_table_addr);
page_table_t* get_pml4();
uint64_t read_cr3(void);

#endif

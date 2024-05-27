#include "page_heap.h"
#include "linked_list_allocator.h"

#define BLOCK_SIZE 4096
linked_list_allocator_t* page_heap;

// Simple heap that allocates 4096 bytes for anything

void initialize_page_heap(void* start, size_t len) {
    page_heap = start;
    size_t allocator_length = len / BLOCK_SIZE;
    initialize_linked_list_allocator(page_heap, allocator_length, page_heap + allocator_length + 1, BLOCK_SIZE, page_heap + 1);
}

void* malloc(size_t bytes) {
    return linked_list_malloc(page_heap, bytes);
}

void free(void* address) {
    linked_list_free(page_heap, address);
}

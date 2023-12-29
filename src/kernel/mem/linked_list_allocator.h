#ifndef LINKED_LIST_ALLOCATOR_H
#define LINKED_LIST_ALLOCATOR_H

#include "physical_memory_allocator.h"

typedef struct linked_list_allocator_t {
	physical_memory_allocator_t allocator;
	size_t block_length;
	size_t ptr;
	size_t* free_list;
} linked_list_allocator_t;

void* linked_list_malloc(physical_memory_allocator_t* allocator, size_t size);
void linked_list_free(physical_memory_allocator_t* allocator, void* address);
void initialize_linked_list_allocator(linked_list_allocator_t* allocator_ptr, size_t capacity, void* base, size_t block_length, size_t* free_list);

#endif

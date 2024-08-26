#ifndef LINKED_LIST_ALLOCATOR_H
#define LINKED_LIST_ALLOCATOR_H

#include "physical_memory_allocator.h"

typedef struct linked_list_node_t {
	struct linked_list_node_t* next;
} linked_list_node_t;

typedef struct memory_allocation_t {
	linked_list_node_t* next;
	physical_memory_allocator_t* allocator;
} memory_allocation_t;

typedef struct linked_list_allocator_t {
	physical_memory_allocator_t allocator;
	size_t block_length;
	linked_list_node_t* tail;
} linked_list_allocator_t;

void* linked_list_malloc(physical_memory_allocator_t* allocator, size_t size);
void linked_list_free(void* address);
void initialize_linked_list_allocator(linked_list_allocator_t* allocator_ptr, size_t capacity, void* base, size_t block_length);

#endif

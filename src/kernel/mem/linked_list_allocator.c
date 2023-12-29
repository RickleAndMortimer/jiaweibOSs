#include "linked_list_allocator.h"
#include "physical_memory_allocator.h"
#include <stdbool.h>
#include <string.h>

void* linked_list_malloc(physical_memory_allocator_t* allocator, size_t size) {
	linked_list_allocator_t* ll_allocator = (linked_list_allocator_t*) allocator;
	if (size + ll_allocator->allocator.memory_used > ll_allocator->allocator.memory_capacity) {
		return NULL;
	}

	uint8_t* address = ll_allocator->allocator.base + ll_allocator->block_length * ll_allocator->ptr;
	ll_allocator->ptr = ll_allocator->free_list[ll_allocator->ptr];
	return (void*) address;
}

void linked_list_free(physical_memory_allocator_t* allocator, void* address) {
	linked_list_allocator_t* ll_allocator = (linked_list_allocator_t*) allocator;
	uintptr_t diff = (uintptr_t) address - (uintptr_t) ll_allocator->allocator.base;

	bool valid_address = diff % ll_allocator->block_length == 0;

	if (valid_address) {
		size_t index_to_be_freed = diff / ll_allocator->block_length;
		size_t tmp = ll_allocator->free_list[ll_allocator->ptr];
		ll_allocator->free_list[ll_allocator->ptr] = index_to_be_freed;
		ll_allocator->free_list[index_to_be_freed] = tmp;
		memset(address, 0, ll_allocator->block_length);
	}
}

void initialize_linked_list_allocator(linked_list_allocator_t* allocator_ptr, size_t capacity, void* base, size_t block_length, size_t* free_list) {
	initialize_physical_memory_allocator((physical_memory_allocator_t*) allocator_ptr, capacity, linked_list_malloc, linked_list_free, base);
	allocator_ptr->block_length = block_length;
	allocator_ptr->ptr = 0;
	allocator_ptr->free_list = free_list;

	for (size_t i = 0; i < capacity - 1; i++) {
		free_list[i] = i + 1;	
	}

	free_list[capacity - 1] = 0;	
}

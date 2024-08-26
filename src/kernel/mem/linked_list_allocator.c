#include "linked_list_allocator.h"
#include "physical_memory_allocator.h"
#include <stdbool.h>
#include <string.h>

void* linked_list_malloc(physical_memory_allocator_t* allocator, size_t size) {
	memory_allocation_t* alloc;
	linked_list_allocator_t* ll_allocator = (linked_list_allocator_t*) allocator;
	void* addr = (char*) ll_allocator->tail + sizeof(memory_allocation_t);

	if (size + ll_allocator->allocator.memory_used > ll_allocator->allocator.memory_capacity) {
		return NULL;
	}

	if (ll_allocator->tail->next == NULL) {
		alloc = (char*) ll_allocator->tail + sizeof(memory_allocation_t) + ll_allocator->block_length;
		alloc->next = NULL;
		alloc->allocator = ll_allocator;
		ll_allocator->tail->next = alloc;
	}  else {
		alloc = ll_allocator->tail->next;
	}
	ll_allocator->tail = alloc;
	return addr;
}

void linked_list_free(void* address) {
	memory_allocation_t* alloc = (char*) address - sizeof(memory_allocation_t);
	linked_list_allocator_t* ll_allocator = alloc->allocator;
	alloc->next = ll_allocator->tail->next;
	ll_allocator->tail = alloc;
}

void initialize_linked_list_allocator(linked_list_allocator_t* allocator_ptr, size_t capacity, void* base, size_t block_length) {
	initialize_physical_memory_allocator((physical_memory_allocator_t*) allocator_ptr, capacity, linked_list_malloc, linked_list_free, base);
	allocator_ptr->block_length = block_length;
	allocator_ptr->tail = base;

	memory_allocation_t* alloc = base;
	alloc->allocator = allocator_ptr;
	alloc->next = NULL;
}

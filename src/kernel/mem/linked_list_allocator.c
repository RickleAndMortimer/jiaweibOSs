#include "linked_list_allocator.h"
#include "physical_memory_allocator.h"

void* linked_list_malloc(physical_memory_allocator_t* allocator, size_t size) {
	linked_list_allocator_t* ll_allocator = (linked_list_allocator_t*) allocator;
	if (size + ll_allocator->allocator.memory_used > ll_allocator->allocator.memory_capacity) {
		return NULL;
	}
	ll_allocator->allocator.memory_used += ll_allocator->block_length;
	return ll_allocator->allocator.base + ll_allocator->block_length * ll_allocator->ptr++;
}

void linked_list_free(physical_memory_allocator_t* allocator, void* address) {

}

void initialize_linked_list_allocator(linked_list_allocator_t* allocator_ptr, size_t capacity, void* base, size_t block_length) {
	initialize_physical_memory_allocator((physical_memory_allocator_t*) allocator_ptr, capacity, linked_list_malloc, linked_list_free, base);
	allocator_ptr->block_length = block_length;
	allocator_ptr->ptr = 0;
}

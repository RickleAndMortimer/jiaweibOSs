#include "physical_memory_allocator.h"

void initialize_physical_memory_allocator(
	physical_memory_allocator_t* allocator_ptr, 			  
	size_t capacity,  
	void* (malloc)(physical_memory_allocator_t* allocator, size_t size), 	  
	void (free)(physical_memory_allocator_t* allocator, void* address),
	void* base	
) {
	allocator_ptr->memory_capacity = capacity;	
	allocator_ptr->memory_used = 0;	
	allocator_ptr->malloc = malloc;	
	allocator_ptr->free = free;	
	allocator_ptr->base = base;
}

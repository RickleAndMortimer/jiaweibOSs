#include "physical_memory_allocator.h"

#define PAGE_SIZE 4096

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

/*
 * Setup:
 * 1. Get total blocks available with the smallest page size
 * 2. Create bitmaps for each possible page size
 * 3. Return the buddy allocator ready for use
 * */
void initialize_buddy_allocator() {

}

/*
 * Buddy allocation algorithm:
 * 1. Calculate the smallest page size that fits the memory requested
 * 2. Use the bitmap of that page size to find an unused memory location
 * 3. Mark the index of the free memory as used.
 * 4. Mark the smaller memory locations as used for the entire region
 * 5. Mark
 * */

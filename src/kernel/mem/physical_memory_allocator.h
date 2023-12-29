#ifndef PMM_H 
#define PMM_H

#include <stdint.h>
#include <stddef.h>

typedef struct physical_memory_allocator_t {
	size_t memory_capacity;
	size_t memory_used;
	void* (*malloc)(struct physical_memory_allocator_t* allocator, size_t size);
	void (*free)(struct physical_memory_allocator_t* allocator, void* address);
	void* base;
} physical_memory_allocator_t;

void initialize_physical_memory_allocator(
	physical_memory_allocator_t* allocator_ptr, 			  
	size_t capacity,  
	void* (malloc)(physical_memory_allocator_t* allocator, size_t size), 	  
	void (free)(physical_memory_allocator_t* allocator, void* address),
	void* base
);

#endif

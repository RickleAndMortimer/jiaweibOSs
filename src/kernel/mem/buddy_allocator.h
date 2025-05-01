#ifndef BUDDY_ALLOCATOR_H
#define BUDDY_ALLOCATOR_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

enum block_state {
	BLOCK_USED,
	BLOCK_FREE,
	BLOCK_SPLIT
};

typedef struct buddy_free_node_t {
  size_t size;
  size_t index;
  uint8_t state;
} buddy_free_node_t;

typedef struct buddy_allocator_t {
  size_t used;
  size_t capacity;
  size_t min_block_size;
  void* pool;
  buddy_free_node_t* free_list;
} buddy_allocator_t;

void* buddy_alloc(buddy_allocator_t* alloc, size_t request_size);
void buddy_free(buddy_allocator_t* alloc, void* addr);
void buddy_init(buddy_allocator_t* allocator, buddy_free_node_t* free_list, size_t capacity, size_t min_block_size, void* pool);

#endif

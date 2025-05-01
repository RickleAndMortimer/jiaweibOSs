#include "buddy_allocator.h"
#include "physical_memory_allocator.h"
#include <stdbool.h>
#include <string.h>
#include <printf/printf.h>

static buddy_free_node_t* left(buddy_allocator_t* alloc, buddy_free_node_t* node) {
	if (2 * node->index + 1 > alloc->capacity / alloc->min_block_size) {
		return NULL;
	}
	return alloc->free_list + 2 * node->index + 1;
}

static buddy_free_node_t* right(buddy_allocator_t* alloc, buddy_free_node_t* node) {
	if (2 * node->index + 2 > alloc->capacity / alloc->min_block_size) {
		return NULL;
	}
	return alloc->free_list + 2 * node->index + 2;
}

static void* B2A(buddy_allocator_t* alloc, buddy_free_node_t* block) {
	return (char*) alloc->pool + (block->index - alloc->capacity / block->size + 1) * block->size;
}

static buddy_free_node_t* A2B(buddy_allocator_t* alloc, void* addr) {
	uintptr_t rel_addr = (uintptr_t) addr - (uintptr_t) alloc->pool;
	if (rel_addr % alloc->min_block_size != 0) {
		printf("bad address%d\n");
		return NULL;
	}

	uint64_t block_size = alloc->capacity;
	while (rel_addr % block_size != 0) {
		block_size >>= 1;
	}
	uint64_t index = rel_addr / block_size;
	buddy_free_node_t* node = alloc->free_list + (alloc->capacity / block_size - 1 + index);

	while (node->state == BLOCK_USED && left(alloc, node)) {
		node = left(alloc, node);
	}

	return node;
}

static buddy_free_node_t* parent(buddy_allocator_t* alloc, buddy_free_node_t* node) {
	if (node->index == 0) {
		return node;
	}
	return alloc->free_list + (node->index - 1) / 2;
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))
void* buddy_alloc(buddy_allocator_t* alloc, size_t request_size) {
	size_t alloc_size = request_size;
	buddy_free_node_t* node = alloc->free_list;
	while (node->size > alloc_size) {
		buddy_free_node_t* left_node = left(alloc, node);
		buddy_free_node_t* right_node = right(alloc, node);
		if (left_node->state == BLOCK_FREE || (left_node->state == BLOCK_SPLIT && left_node->size != alloc_size)) {
			node = left_node;
		} else if (right_node->state == BLOCK_FREE || (right_node->state == BLOCK_SPLIT && right_node->size != alloc_size)) {
			node = right_node;
		} else if (node->size != alloc_size) {
			node->state = BLOCK_SPLIT;
			left_node->state = BLOCK_FREE;
			right_node->state = BLOCK_FREE;
			node = left_node;
		} else {
			return NULL;
		}
	}

	node->state = BLOCK_USED;
	if (right(alloc, parent(alloc, node))->state == BLOCK_USED) {
		parent(alloc, node)->state = BLOCK_USED;
	}
	alloc->used += node->size;

	return B2A(alloc, node);
}

void buddy_free(buddy_allocator_t* alloc, void* addr) {
	buddy_free_node_t* node = A2B(alloc, addr);

	node->state = BLOCK_FREE;
	alloc->used -= node->size;
	node = parent(alloc, node);
	while ((!left(alloc, node) && !right(alloc, node)) || (left(alloc, node)->state == BLOCK_FREE && right(alloc, node)->state == BLOCK_FREE)) {
		node->state = BLOCK_FREE;
		if (node == parent(alloc, node)) {
			break;
		}
		node = parent(alloc, node);
	}
}

void buddy_init(buddy_allocator_t* allocator, buddy_free_node_t* free_list, size_t capacity, size_t min_block_size, void* pool) {
	allocator->free_list = free_list;
	allocator->capacity = capacity;
	allocator->min_block_size = min_block_size;
	allocator->pool = pool;
	size_t x = 0; 
	size_t y = 0; 
	while ((1 << x) * min_block_size <= capacity) {
		for (size_t i = 0; i < 1 << x; i++) {
			free_list[y].size = capacity >> x;
			free_list[y].index = y;
			free_list[y].state = BLOCK_USED;
			y++;
		}
		x++;
	}
	free_list[0].state = BLOCK_FREE;
}

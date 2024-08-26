#include "heap.h"

static volatile linked_list_allocator_t* allocators[13];

// creates memory allocators 
void initialize_linked_list_heap(linked_list_heap_request_t* list, size_t length, void* base) {
  char* ptr = base;
  for (size_t i = 0; i < length; i++) {
    initialize_linked_list_allocator((linked_list_allocator_t*) ptr, list[i].number_of_blocks, (char*) ptr + sizeof(linked_list_allocator_t), list[i].block_size);
    allocators[i] = ptr;
    ptr += sizeof(linked_list_allocator_t) + list[i].number_of_blocks * (list[i].block_size + sizeof(memory_allocation_t));
  }
}

size_t get_linked_list_allocator_by_length(length) {
  size_t zeroes = __builtin_clzll(length);
  if (1 << (63 ^ zeroes) == length) {
    return 63 ^ zeroes;
  }
  return (63 ^ zeroes) + 1;
}

void* linked_list_heap_fast_malloc(size_t length) {
  if (length == 0) {
    return NULL;
  }
  size_t pos = get_linked_list_allocator_by_length(length);
  return linked_list_malloc(allocators[pos], length);
}

void linked_list_heap_free(void* address) {
  linked_list_free(address);
}

void initialize_default_request_heap_sizes(void* base) {
  linked_list_heap_request_t requests[] = {
    {
      .block_size = 1,
      .number_of_blocks =  1000
    },
    {
      .block_size = 2,
      .number_of_blocks = 1000
    },
    {
      .block_size = 4,
      .number_of_blocks = 1000
    },
    {
      .block_size = 8,
      .number_of_blocks = 1000
    },
    {
      .block_size = 16,
      .number_of_blocks = 1000
    },
    {
      .block_size = 32,
      .number_of_blocks = 1000
    },
    {
      .block_size = 64,
      .number_of_blocks = 1000
    },
    {
      .block_size = 128,
      .number_of_blocks = 1000
    },
    {
      .block_size = 256,
      .number_of_blocks = 1000
    },
    {
      .block_size = 512,
      .number_of_blocks = 1000
    },
    {
      .block_size = 1024,
      .number_of_blocks = 1000
    },
    {
      .block_size = 2048,
      .number_of_blocks = 1000
    },
    {
      .block_size = 4096,
      .number_of_blocks = 1000
    }
  };
  initialize_linked_list_heap(requests, sizeof(requests) / sizeof(linked_list_heap_request_t), base);
} 

void* realloc(void* address, size_t bytes) {
    void* new_address = linked_list_heap_fast_malloc(bytes);
    memcpy(new_address, address, bytes);
    linked_list_heap_free(address);
    return new_address;
}

void* malloc(size_t bytes) {
    return linked_list_heap_fast_malloc(bytes);
}

void free(void* address) {
    linked_list_heap_free(address);
}

#include "heap.h"

// creates memory allocators 
void initialize_linked_list_heap(linked_list_heap_request_t* list, size_t length, void* base, size_t memory_length) {
  uintptr_t ptr = base;
  for (size_t i = 0; i < length; i++) {
    initialize_linked_list_allocator_aligned((linked_list_allocator_t*) ptr, list[i].number_of_blocks, list[i].block_size);
    ptr += sizeof(linked_list_allocator_t) + sizeof(size_t) * list[i].number_of_blocks + list[i].number_of_blocks * list[i].block_size;
  }
}

void initialize_default_request_heap_sizes(void* base, size_t memory_length) {
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
  initialize_linked_list_heap(requests, sizeof(requests) / 8, base, memory_length);
} 

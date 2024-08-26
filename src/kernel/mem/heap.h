#include <stdint.h>
#include "linked_list_allocator.h"

typedef struct linked_list_heap_request_t {
  size_t block_size;
  size_t number_of_blocks;
} linked_list_heap_request_t;

void initialize_linked_list_heap(linked_list_heap_request_t* list, size_t length, void* base);
void* linked_list_heap_fast_malloc(size_t length);
void initialize_default_request_heap_sizes(void* base);

void* malloc(size_t bytes);
void free(void* address);

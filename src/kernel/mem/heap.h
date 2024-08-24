#include <stdint.h>
#include "linked_list_allocator.h"

typedef struct linked_list_heap_request_t {
  size_t block_size;
  size_t number_of_blocks;
} linked_list_heap_request_t;

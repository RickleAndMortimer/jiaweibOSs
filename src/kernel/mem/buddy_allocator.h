#include <stdint.h>
#include <stdlib.h>

typedef struct linked_list_node_t {
  linked_list_node_t* next;
} linked_list_node_t;

typedef struct buddy_struct_t {
  linked_list_node_t head;
  size_t* map;
} buddy_struct_t;



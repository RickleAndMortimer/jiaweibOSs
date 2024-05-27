#include <stdint.h>
#include <stddef.h>

void initialize_page_heap(void* start, size_t len);
void* malloc(size_t bytes);
void free(void* address);

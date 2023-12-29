#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>
#include <sys/io.h>

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end); 
void disable_cursor(); 
void update_cursor(int x, int y);
uint16_t get_cursor_position(void);

#endif

#ifndef TERM_H
#define TERM_H

#include <flanterm/flanterm.h>
#include <flanterm/backends/fb.h>
#include "../limine.h"

static struct flanterm_context* ft_ctx;
struct flanterm_context* initialize_terminal(struct limine_framebuffer* framebuffer);
void kprint(const char* msg, size_t len);
void ftprint(struct flanterm_context* ft_ctx, const char* msg, size_t len);
void _put_char(char character);

#endif

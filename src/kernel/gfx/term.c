#include "term.h"

static struct flanterm_context* ft_ctx;

struct flanterm_context* initialize_terminal(struct limine_framebuffer* framebuffer) {
    ft_ctx = flanterm_fb_init(
        NULL,
        NULL,
        framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch,
        framebuffer->red_mask_size, framebuffer->red_mask_shift,
        framebuffer->green_mask_size, framebuffer->green_mask_shift,
        framebuffer->blue_mask_size, framebuffer->blue_mask_shift,
        NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, 0, 0, 1,
        0, 0,
        0
    );
    return ft_ctx;
}

void kprint(const char* msg, size_t len) {
    flanterm_write(ft_ctx, msg, len);
}

void ftprint(struct flanterm_context* ft_ctx, const char* msg, size_t len) {
    flanterm_write(ft_ctx, msg, len);
}

void _putchar(char character) {
    ft_ctx->raw_putchar(ft_ctx, character);
}

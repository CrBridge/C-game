#ifndef WINDOW_H
#define WINDOW_H

#include "types.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>

#include <stdio.h>

i32 window_init_window(u16 width, u16 height, char* name);
void window_clean_window(void);
void window_swap_buffer(void);
f32 window_get_aspect(void);
u16 window_get_width(void);
u16 window_get_height(void);

#endif
#ifndef WINDOW_H
#define WINDOW_H

#include "types.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>

#include <stdio.h>

// gonna go simple for now, if something seems like it can be grouped up
// e.g. window + context (+ other window info like width/height), I'll 
// make a file for it, with a struct etc.
typedef struct app {
	SDL_Window* pWindow;
	SDL_GLContext context;
	u16 width;
	u16 height;
} App;

int init_window(u16 width, u16 height, char* name);
void clean_window(void);
void swap_buffer(void);

#endif
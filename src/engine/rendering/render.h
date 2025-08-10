#ifndef RENDER_H
#define RENDER_H

#include <SDL3/SDL.h>
#include <linmath.h>

#include "camera.h"
#include "../types.h"
#include "../math_util.h"

typedef struct render_state {
	SDL_Window* window;
	i16 width;
	i16 height;
} Render_State;

void render_init(void);
void render_begin(void);
void render_end(void);
void render_quad(vec2 pos, vec2 size, vec4 color);

// TODO! find a better place to put these
void update_camera_position(Movement_Direction direction, f32 dt);
void update_camera_target(f32 dx, f32 dy);

#endif
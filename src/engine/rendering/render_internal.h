#ifndef RENDER_INTERNAL_H
#define RENDER_INTERNAL_H

#include <SDL3/SDL.h>

#include "../types.h"
#include "render.h"

typedef struct render_state_internal {
	u32 vao_quad;
	u32 vbo_quad;
	u32 ebo_quad;
	u32 shader_default;
	u32 texture_color;
	mat4x4 projection;
}Render_State_Internal;

SDL_Window* render_init_window(i16 width, i16 height);
void render_init_quad(u32* vao, u32* vbo, u32* ebo);
void render_init_image_texture(u32* texture);
void render_init_shaders(Render_State_Internal* state);
u32 render_shader_create(const char* vert_path, const char* frag_path);

#endif

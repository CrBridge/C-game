#ifndef TEXTURE_H
#define TEXTURE_H

#include <stb_image.h>
#include <glad/glad.h>

#include "../types.h"
#include "../util.h"

typedef u32 tex_id;

typedef struct texture {
	u32 id;
	u16 width;
	u16 height;
} Texture;

void texture_init(Texture* id);
void texture_load_texture(Texture* id, const char* texturePath, int flip);
void texture_load_cube_texture(Texture* id, const char** texturePaths);
void texture_load_from_color(Texture* t, u8 color[4]);
void texture_bind(tex_id* id);
void texture_unbind(void);
void texture_clean(Texture* id);
void texture_delete(tex_id* t);

#endif
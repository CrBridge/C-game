#ifndef TEXTURE_H
#define TEXTURE_H

#include <stb_image.h>
#include <glad/glad.h>

#include "../types.h"
#include "../util.h"

// we might eventually want an easy way to refer to the
// width and height of a texture. At that point it should
// probably become a struct

typedef u32 texture;

void texture_init(texture* id);
void texture_load_texture(texture* id, const char* texturePath);
void texture_load_cube_texture(texture* id, const char** texturePaths);
void texture_bind(texture* id);
void texture_unbind(void);
void texture_clean(texture* id);

#endif
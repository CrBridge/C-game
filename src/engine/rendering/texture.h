#ifndef TEXTURE_H
#define TEXTURE_H

#include <stb_image.h>
#include <glad/glad.h>

#include "../types.h"
#include "../util.h"

// we might eventually want an easy way to refer to the
// width and height of a texture. At that point it should
// probably become a struct
void texture_init(u32* id);
void texture_load_texture(u32* id, const char* texturePath);
void texture_bind(u32* id);
void texture_unbind(void);
void texture_clean(u32* id);

#endif
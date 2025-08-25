#ifndef FBO_H
#define FBO_H

#include "../types.h"
#include "texture.h"

#include <glad/glad.h>

typedef u32 fbo;

void fbo_init(fbo* id);
void fbo_add_buffer(tex_id* id, u16 width, u16 height);
void fbo_bind(fbo* id);
void fbo_unbind(void);
void fbo_clean(fbo* id);

#endif
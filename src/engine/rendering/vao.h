#ifndef VAO_H
#define VAO_H

#include "vbo.h"

typedef u32 vao;

void vao_init(vao* id);
void vao_setup_vbo_attrib(vao* vboId, u32 layout, u32 size, GLenum type, size_t stride, void* offset);
void vao_bind(vao* id);
void vao_unbind(void);
void vao_clean(vao* id);

#endif
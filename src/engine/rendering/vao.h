#ifndef VAO_H
#define VAO_H

#include "vbo.h"

void vao_init(u32* id);
void vao_setup_vbo_attrib(u32* vboId, u32 layout, u32 size, GLenum type, size_t stride, void* offset);
void vao_bind(u32* id);
void vao_unbind(void);
void vao_clean(u32* id);

#endif
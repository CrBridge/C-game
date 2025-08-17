#ifndef RBO_H
#define RBO_H

#include "../types.h"

#include <glad/glad.h>

typedef u32 rbo;

void rbo_init(rbo* id);
void rbo_attach_depth_stencil_buffers(rbo* id, u16 width, u16 height);
void rbo_bind(rbo* id);
void rbo_unbind(void);
void rbo_clean(rbo* id);

#endif
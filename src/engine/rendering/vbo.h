#ifndef VBO_H
#define VBO_H

#include "../types.h"
#include "vertex.h"

#include <glad/glad.h>

// unsigned int for id. Since these things are really only ids,
// structs dont make much sense in my mind. Just use functions?

typedef u32 vbo;

void vbo_init(vbo* id);
void vbo_add_data(vbo* id, Vertex* vertices, u32 vertexCount);
void vbo_bind(vbo* id);
void vbo_unbind(void);
void vbo_clean(vbo* id);

#endif

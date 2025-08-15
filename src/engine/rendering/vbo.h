#ifndef VBO_H
#define VBO_H

#include "../types.h"
#include "vertex.h"

#include <glad/glad.h>

// unsigned int for id. Since these things are really only ids,
// structs dont make much sense in my mind. Just use functions?

void vbo_init(u32* id);
void vbo_add_data(u32* id, Vertex* vertices, u32 vertexCount);
void vbo_bind(u32* id);
void vbo_unbind(void);
void vbo_clean(u32* id);

#endif

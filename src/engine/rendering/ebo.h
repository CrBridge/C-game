#ifndef EBO_H
#define EBO_H

#include "../types.h"

#include <glad/glad.h>

void ebo_init(u32* id);
void ebo_add_data(u32* id, u32* indices, u32 indexCount);
void ebo_bind(u32* id);
void ebo_unbind(void);
void ebo_clean(u32* id);

#endif

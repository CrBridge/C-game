#ifndef EBO_H
#define EBO_H

#include "../types.h"

#include <glad/glad.h>

typedef u32 ebo;

void ebo_init(ebo* id);
void ebo_add_data(ebo* id, u32* indices, u32 indexCount);
void ebo_bind(ebo* id);
void ebo_unbind(void);
void ebo_clean(ebo* id);

#endif

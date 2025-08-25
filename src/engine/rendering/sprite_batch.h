#ifndef SPRITE_BATCH_H
#define SPRITE_BATCH_H

#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "texture.h"
#include "shader.h"
#include "vertex.h"

#include "../data/array.h"
#include "../math/rectangle.h"

#include <linmath.h>

void spritebatch_init(int nativeWidth, int nativeHeight);
void spritebatch_begin(shader* s);
void spritebatch_draw(Rectangle dst, Rectangle src, Texture* t);
void spritebatch_end();
void spritebatch_clean();

#endif
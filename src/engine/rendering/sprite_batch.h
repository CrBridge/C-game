#ifndef SPRITE_BATCH_H
#define SPRITE_BATCH_H

#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "texture.h"
#include "shader.h"
#include "vertex.h"
#include "font.h"

#include "../data/array.h"
#include "../math/rectangle.h"

#include <linmath.h>

void spritebatch_init(int nativeWidth, int nativeHeight);
void spritebatch_begin();
void spritebatch_draw(Rectangle dst, Rectangle src, Texture* t, Vector3f color);
void spritebatch_draw_vec_dst(Vector2f dst, Texture* tex, Vector3f color);
void spritebatch_draw_string(Vector2f dst, Font* f, const char* s, f32 scale, Vector3f color);
void spritebatch_end();
void spritebatch_clean();

#endif
#ifndef RENDERER_H
#define RENDERER_H

#include "../window.h"
#include "../types.h"

#include "../game_object.h"
#include "../components/skybox_component.h"

#include "fbo.h"
#include "rbo.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "sprite_batch.h"

#include <linmath.h>

void renderer_init(int width, int height);
void renderer_begin_frame(mat4x4* projection, mat4x4* view, u32 time, u32 frame);
void renderer_draw_game_object(GameObject* g);
void renderer_draw_skybox(Skybox* s);
void renderer_end_frame();
void renderer_clean();

#endif
#ifndef RENDERER_H
#define RENDERER_H

// what do I want here?
// I think in the .c I want a top-level shader variable
// to store the currently bound shader. Maybe it should be
// a pointer? im not sure. But the point is that lets me know
// what is currently active, and it more easily allows me to use
// this file to call renderer_set_uniforms or some such

// It would also be nice to offload all of the blit stuff to here
// because I think thats a bit of a special case.

// how do scenes fit into this? resolution scaling will be everywhere,
// but I may do post processing in the game I dont want on a menu, so
// what do I do then? I suppose the easiest answer is to simply
// swap out that final shader depending on the context

// so what does init need to do? take in framebuffer info for one, actual uniform
// info will need to be passed in (time, camera etc.)

#include "../window.h"
#include "../types.h"

#include "fbo.h"
#include "rbo.h"
#include "mesh.h"
#include "texture.h"

//TODO - some kind of set_shader function
//	state will track the currently bound shader
//	so I can have some kind of renderer_draw_game_object
//	without having to pass in proj and view
//	the flow could be set_shader,
//	then I have functions similar to shader_set_mat4 etc.
//	to pass those in.

void renderer_init(int width, int height);
void renderer_begin_frame();
void renderer_end_frame();
void renderer_clean();

#endif
// I do still like the idea of doing a game, it just has to be very simply
// very important - im not making an engine here, just a game
// so my implementation can be as niche and unversatile as I want
// as long as it gets it done :)

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "components/transform_component.h"
#include "rendering/mesh.h"
#include "rendering/texture.h"
#include "rendering/shader.h"

// would be nice to store a reference to the shader
// thats going to use it, but then I'll often be binding repeatedly when
// it isn't needed, and how would I know if the referenced shader is the one already
// bound. I think I'd need a more robust render system. Though in essence, its as simple
// as tracking a u32 and in this draw function we simply check it, and if different
// reassign the value + the shaders uniforms. This is doubly needed as without
// the shader ref, I can't use transform in draw either,since there is
// no uniform to set

typedef struct gameObject {
	Transform transform;
	Mesh mesh;
	texture texId;
} GameObject;

void game_object_init(GameObject* g);
void game_object_draw(GameObject* g);
void game_object_clean(GameObject* g);

#endif
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
// wrote more on this in renderer, but having it store a render_type enum value
// that controls how its drawn via a switch statement could be a solution

typedef struct gameObject {
	Transform transform;
	Mesh mesh;
	Texture texture;
	//u8 selected; debug
} GameObject;

void game_object_init(GameObject* g);
void game_object_draw(GameObject* g);
// allows passing in the mode used to draw the mesh
// e.g. GL_LINES, GL_POINTS, etc.
void game_object_draw_debug(GameObject* g, GLenum mode);
void game_object_clean(GameObject* g);

#endif
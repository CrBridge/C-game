#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "components/transform.h"
#include "rendering/mesh.h"
#include "rendering/texture.h"
#include "rendering/shader.h"

// NOTE: not all values here should be used on a game object, e.g. blit, control & sky
//	they are just here to keep consistency with some code in renderer
typedef enum {
	RENDER_DEFAULT,
	RENDER_DEBUG,
	RENDER_SHELL,
	RENDER_TERRAIN,
	RENDER_SKY,
	RENDER_CONTROL,
	RENDER_BLIT
} RenderType;

typedef struct gameObject GameObject;

typedef void (*game_object_draw_func)(GameObject* g);
typedef void (*game_object_update_func)(GameObject* g, f32 dt);
typedef void (*game_object_input_func)(GameObject* g, f32 dt);

typedef struct gameObject {
	Transform transform;
	Mesh mesh;
	Texture texture;
	RenderType type;

	// function pointers. If I end up wanting more, it might be worth
	// making a vtable, pretty much just storing all the function pointers
	// in a struct that I can then put here

	game_object_draw_func draw;
	game_object_update_func update;
	game_object_input_func input;

	void* additional_data;
} GameObject;

void game_object_init(GameObject* g, RenderType type);
void game_object_draw(GameObject* g);
// allows passing in the mode used to draw the mesh
// e.g. GL_LINES, GL_POINTS, etc.
void game_object_draw_debug(GameObject* g, GLenum mode);
void game_object_clean(GameObject* g);

#endif
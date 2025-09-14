#include "game_object.h"

void game_object_init(GameObject* g, RenderType r, ObjectType o) {
	component_transform_init(&g->transform);
	texture_init(&g->texture);
	g->type = r;
	g->obj_type = o;

	g->input = NULL;
	g->update = NULL;
	g->draw	= NULL;
	g->additional_data = NULL;
}

void game_object_draw(GameObject* g) {
	texture_bind(&g->texture.id);
	mesh_draw(&g->mesh);
}

void game_object_draw_debug(GameObject* g, GLenum mode) {
	texture_bind(&g->texture.id);
	mesh_draw_debug(&g->mesh, mode);
}

void game_object_clean(GameObject* g) {
	mesh_clean(&g->mesh);
	texture_clean(&g->texture);

	g->input = NULL;
	g->update = NULL;
	g->additional_data = NULL;
}

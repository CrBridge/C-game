#include "game_object.h"

void game_object_init(GameObject* g) {
	component_transform_init(&g->transform);
	texture_init(&g->texId);
}

void game_object_draw(GameObject* g) {
	// no shader for now, just assume main does it
	texture_bind(&g->texId);
	mesh_draw(&g->mesh);
}

void game_object_clean(GameObject* g) {
	mesh_clean(&g->mesh);
	texture_clean(&g->texId);
}

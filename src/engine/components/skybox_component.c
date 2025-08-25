#include "skybox_component.h"

Skybox skybox_init(const char** texturePaths) {
	Skybox s = {0};
	mesh_load_sky_cube(&s.cube);
	texture_init(&s.cubemap);
	texture_load_cube_texture(&s.cubemap, texturePaths);

	return s;
}

void skybox_draw(Skybox* s) {
	texture_bind(&s->cubemap.id);
	mesh_draw(&s->cube);
}

void skybox_clean(Skybox* s) {
	mesh_clean(&s->cube);
	texture_clean(&s->cubemap);
}

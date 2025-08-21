#include "transform_component.h"

void component_transform_init(Transform* t) {
	memcpy(t->position, (vec3) { 0, 0, 0 }, sizeof(vec3));
	memcpy(t->rotation, (vec3) { 0, 0, 0 }, sizeof(vec3));
	t->scale = 1.0f;
}

void component_transform_calculate_model_matrix(vec4* model, Transform* t) {
	mat4x4_translate(model, t->position[0], t->position[1], t->position[2]);

	mat4x4_rotate(model, model, 1, 0, 0, degree_to_rad(t->rotation[0]));
	mat4x4_rotate(model, model, 0, 1, 0, degree_to_rad(t->rotation[1]));
	mat4x4_rotate(model, model, 0, 0, 1, degree_to_rad(t->rotation[2]));

	// use aniso to retain the w component at 1.0
	mat4x4_scale_aniso(model, model, t->scale, t->scale, t->scale);
}

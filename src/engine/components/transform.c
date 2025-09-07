#include "transform.h"

void component_transform_init(Transform* t) {
	memcpy(t->position, (vec3) { 0, 0, 0 }, sizeof(vec3));
	mat4x4_identity(t->rotation);
	t->scale = 1.0f;
}

void component_transform_calculate_model_matrix(vec4* model, Transform* t) {
	mat4x4_translate(model, t->position[0], t->position[1], t->position[2]);

	mat4x4_mul(model, model, t->rotation);

	// use aniso to retain the w component at 1.0
	mat4x4_scale_aniso(model, model, t->scale, t->scale, t->scale);
}

void component_transform_get_local_forwards(vec3 f, Transform* t) {
	f[0] = t->rotation[2][0];
	f[1] = t->rotation[2][1];
	f[2] = t->rotation[2][2];
}

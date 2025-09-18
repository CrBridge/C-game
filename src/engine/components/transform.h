#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "../types.h"
#include "../math_util.h"
#include "../math/vector.h"

#include <linmath.h>

// we're using scale for the transform, as allowing individual scaling by the
// xyz axes would mean we have to account for it in the normals

typedef struct transform {
	//vec3 position;
	Vector3f position;
	mat4x4 rotation;
	//quat rotation;
	f32 scale;
} Transform;

// Initialise the transform component with default values. This will be
// a position of {0, 0, 0}, a rotation of {0, 0, 0}, and a scale of 1.0
void component_transform_init(Transform* t);
void component_transform_calculate_model_matrix(vec4* model, Transform* t);
void component_transform_get_local_forwards(vec3 f, Transform* t);

#endif
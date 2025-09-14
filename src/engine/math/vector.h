#ifndef VECTOR_H
#define VECTOR_H

#include "../types.h"

// The benefit of these vectors is that they are identical in size to linmaths vec2/3.
//	so hypothetically, I should still be able to use them in linmath functions,
//	but I'll have to test that.

typedef union {
	struct { f32 x, y, z; };
	f32 arr[3];
} Vector3f;

typedef union {
	struct { f32 x, y; };
	f32 arr[2];
} Vector2f;

// adds 2 vector3s by their individual components 
Vector3f vector3f_add_vector3f(Vector3f a, Vector3f b);
// adds a value to a vector3 on all of its components 
Vector3f vector3f_add_component(Vector3f a, f32 b);
// multiplies 2 vector3s by their individual components
Vector3f vector3f_mul_vector3f(Vector3f a, Vector3f b);
// multiplies the components of a vector3 by a value
Vector3f vector3f_mul_component(Vector3f a, f32 b);
// divides 2 vector3s by their individual components 
Vector3f vector3f_div_vector3f(Vector3f a, Vector3f b);
// divides a vector3 by a value on all of its components 
Vector3f vector3f_div_component(Vector3f a, f32 b);
// divides a vector2 by a value on all of its components 
Vector2f vector2f_div_component(Vector2f a, f32 b);

#endif
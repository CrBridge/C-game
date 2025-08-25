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

#endif
#ifndef CUBE_H
#define CUBE_H

#include "vector.h"

typedef struct cube {
	int x, y, z;
	int width, height, depth;
} Cube;

Cube cube_init(int x, int y, int z, int width, int height, int depth);
int cube_is_vector3f_intersecting(Cube c, Vector3f v);

#endif
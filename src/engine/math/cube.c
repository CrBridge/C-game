#include "cube.h"

Cube cube_init(int x, int y, int z, int width, int height, int depth) {
	Cube c = { 0 };
	c.x = x;
	c.y = y;
	c.z = z;

	c.width = width;
	c.height = height;
	c.depth = depth;

	return c;
}

int cube_is_vector3f_intersecting(Cube c, Vector3f v) {
	int is_intersecting =
		v.x >= c.x && v.x <= c.x + c.width &&
		v.y >= c.y && v.y <= c.y + c.height &&
		v.z >= c.z && v.z <= c.z + c.depth;

	return is_intersecting;
}
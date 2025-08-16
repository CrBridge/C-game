#include "vertex.h"

int vertices_are_equal(Vertex* a, Vertex* b) {
	if (a->position[0] == b->position[0] &&
		a->position[1] == b->position[1] &&
		a->position[2] == b->position[2] &&
		a->normal[0] == b->normal[0] &&
		a->normal[1] == b->normal[1] &&
		a->normal[2] == b->normal[2] &&
		a->uv[0] == b->uv[0] &&
		a->uv[1] == b->uv[1]
	) {
		return 1; 
	}

	return 0;
}
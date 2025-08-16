#ifndef VERTEX_H
#define VERTEX_H

#include <linmath.h>

typedef struct vertex {
	vec3 position;
	vec3 normal;
	vec2 uv;
} Vertex;

// equals method? Do I need that? Might do when model loading
// for vertex duplication prevention
int vertices_are_equal(Vertex* a, Vertex* b);

#endif
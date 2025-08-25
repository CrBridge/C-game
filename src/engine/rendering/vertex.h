#ifndef VERTEX_H
#define VERTEX_H

#include "../types.h"
#include "../math/vector.h"

#include <linmath.h>

#include <string.h>

typedef struct vertex {
	Vector3 position;
	Vector3 normal;
	Vector2 uv;
} Vertex;

// equals method? Do I need that? Might do when model loading
// for vertex duplication prevention
int vertices_are_equal(Vertex* a, Vertex* b);
size_t vertices_map_hash(const void* key, size_t key_size);
int vertices_map_eq(const void* a, size_t a_size, const void* b, size_t b_size);

#endif
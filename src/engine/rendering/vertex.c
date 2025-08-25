#include "vertex.h"

//TODO! should add to math_util some basic vec3_equal vec2_equal etc.
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

static inline size_t fnv1a_update(size_t hash, const void* data, size_t len) {
	const u8* bytes = (const u8*)data;
	for (size_t i = 0; i < len; i++) {
		hash ^= bytes[i];
		hash *= 1099511628211ull;
	}
	return hash;
}

size_t vertices_map_hash(const void* key, size_t key_size) {
	const Vertex* v = key;

	size_t hash = 1469598103934665603ull;
	hash = fnv1a_update(hash, v->position, sizeof(v->position));
	hash = fnv1a_update(hash, v->normal, sizeof(v->normal));
	hash = fnv1a_update(hash, v->uv, sizeof(v->uv));

	return hash;
}

int vertices_map_eq(const void* a, size_t a_size, const void* b, size_t b_size) {
	Vertex* v1 = (Vertex*)a;
	Vertex* v2 = (Vertex*)b;
	
	return vertices_are_equal(v1, v2);
}

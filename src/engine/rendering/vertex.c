#include "vertex.h"

//TODO! should add to math_util some basic vec3_equal vec2_equal etc.
int vertices_are_equal(Vertex* a, Vertex* b) {
	if (a->position.x == b->position.x &&
		a->position.y == b->position.y &&
		a->position.z == b->position.z &&
		a->normal.x == b->normal.x &&
		a->normal.y == b->normal.y &&
		a->normal.z == b->normal.z &&
		a->uv.x == b->uv.x &&
		a->uv.y == b->uv.y
	) {
		return 1; 
	}

	return 0;
}

//TODO! I get a pretty high collision rate on
//	using these for my map. Given the hardcoded map size
//	is far greater than the number of vertices I put in
//	I'm pretty sure this hash function isn't great
//	for context - given ~4500 vertices, I get ~1000 collisions

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
	hash = fnv1a_update(hash, &v->position, sizeof(v->position));
	hash = fnv1a_update(hash, &v->normal, sizeof(v->normal));
	hash = fnv1a_update(hash, &v->uv, sizeof(v->uv));

	return hash;
}

int vertices_map_eq(const void* a, size_t a_size, const void* b, size_t b_size) {
	Vertex* v1 = (Vertex*)a;
	Vertex* v2 = (Vertex*)b;
	
	return vertices_are_equal(v1, v2);
}

#ifndef MAP_H
#define MAP_H

#include "../types.h"
#include "../util.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_SIZE 10000

//TODO! we're hardcoding map size right now, so any instance will have this size
// most would be better with more or less. Starting slower and supporting
// dynamic growth would be alot better

typedef size_t (*hash_func_t)(const void* key, size_t key_size);
typedef int (*eq_func_t)(const void* a, size_t a_size, const void* b, size_t b_size);

typedef struct entry {
	void* key;
	size_t key_size;
	void* value;
	size_t value_size;
	struct entry* next;
} Entry;

typedef struct map {
	Entry** entries;
	hash_func_t hash_func;
	eq_func_t eq_func;
} Map;

Map* map_create(hash_func_t hash, eq_func_t eq);
Entry* map_pair(const void* key, size_t key_size, const void* value, size_t value_size);
void map_set(Map* m, const void* key, size_t key_size, const void* value, size_t value_size);
void* map_get(Map* m, const void* key, size_t key_size);
void map_free(Map* m);

#endif
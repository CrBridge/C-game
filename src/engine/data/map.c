#include "map.h"

Map* map_create(hash_func_t hash, eq_func_t eq) {
	Map* map = malloc(sizeof(Map));
	if (!map) {
		ERROR_EXIT("Failed to allocate memory for hashmap\n");
	}

	map->entries = calloc(MAP_SIZE, sizeof(Entry*));
	if (!map->entries) {
		free(map);
		ERROR_EXIT("Failed to allocate entries for hashmap\n");
	}

	map->hash_func = hash;
	map->eq_func = eq;

	return map;
}

Entry* map_pair(const void* key, size_t key_size, const void* value, size_t value_size) {
	Entry* entry = malloc(sizeof(Entry));
	if (!entry) {
		ERROR_EXIT("Failed to allocate memory for hashmap entry\n");
	}

	entry->key_size = key_size;
	if (key_size > 0) {
		entry->key = malloc(key_size);
		if (!entry->key) {
			ERROR_EXIT("Failed to allocate memory for hashmap key\n");
		}
		memcpy(entry->key, key, key_size);
	}
	else {
		entry->key = NULL;
	}

	entry->value_size = value_size;
	if (value_size > 0) {
		entry->value = malloc(value_size);
		if (!entry->value) {
			ERROR_EXIT("Failed to allocate memory for hashmap value\n");
		}
		memcpy(entry->value, value, value_size);
	}
	else {
		entry->value = NULL;
	}

	entry->next = NULL;
	return entry;
}

void map_set(Map* m, const void* key, size_t key_size, const void* value, size_t value_size) {
	unsigned int bucket = m->hash_func(key, key_size) % MAP_SIZE;
	Entry* entry = m->entries[bucket];

	if (!entry) {
		m->entries[bucket] = map_pair(key, key_size, value, value_size);
		return;
	}

	Entry* prev;
	while (entry) {
		if (m->eq_func(entry->key, entry->key_size, key, key_size)) {
			free(entry->value);
			entry->value = malloc(value_size);
			if (!entry->value) {
				ERROR_EXIT("Failed to allocate memory for hashmap value\n");
			}
			memcpy(entry->value, value, value_size);
			entry->value_size = value_size;
			return;
		}
		prev = entry;
		entry = entry->next;
	}

	prev->next = map_pair(key, key_size, value, value_size);
}

void* map_get(Map* m, const void* key, size_t key_size) {
	unsigned int bucket = m->hash_func(key, key_size) % MAP_SIZE;
	Entry* entry = m->entries[bucket];

	while (entry) {
		if (m->eq_func(entry->key, entry->key_size, key, key_size)) {
			return entry->value;
		}
		entry = entry->next;
	}

	return NULL;
}

void map_free(Map* m) {
	if (!m) return;

	for (size_t i = 0; i < MAP_SIZE; i++) {
		Entry* entry = m->entries[i];
		while (entry) {
			Entry* next = entry->next;

			free(entry->key);
			free(entry->value);

			free(entry);

			entry = next;
		}
	}

	free(m->entries);
	free(m);
}

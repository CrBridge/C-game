#ifndef ARRAY_H
#define ARRAY_H

#include "../util.h"

#include "stdlib.h"
#include "string.h"

typedef struct array {
	void* data;
	size_t length;
	size_t capacity;
	size_t item_size;
} Array;

Array array_init(size_t item_size);
void array_append(Array* a, void* item);
void* array_get(Array* a, size_t i);
void array_free(Array* a);

#endif
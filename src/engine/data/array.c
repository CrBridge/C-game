#include "array.h"

Array array_init(size_t item_size) {
	Array array = {0};
	array.length = 0;
	array.capacity = 1;
	array.item_size = item_size;
	array.data = malloc(1 * item_size);

	return array;
}

void array_append(Array* a, void* item) {
	if (a->length == a->capacity) {
		size_t new_cap = a->capacity * 2;
		void* tmp = realloc(a->data, new_cap * a->item_size);
		if (!tmp) {
			ERROR_EXIT("Failed to allocate memory for a dynamic array");
		}
		a->data = tmp;
		a->capacity = new_cap;
	}

	void* target = (char*)a->data + (a->length * a->item_size);

	memcpy(target, item, a->item_size);

	a->length++;
}

// get the value at the given index, remember to cast and dereference, e.g. *(int*)array_get(&a, i)
// also note, it may be quicker to just cast .data to the correct type pointer and index directly
void* array_get(Array* a, size_t i) {
	if (i >= a->length) {
		ERROR_EXIT("Attempted to index outside the bounds of the dynamic array\n");
	}
	return (char*)a->data + (i * a->item_size);
}

//TODO! may eventually require a consistent array order, so should have another delete implementation
//	that retains order as well (e.g. left shift elements after index or some such)
void array_remove_swapback(Array* a, size_t i) {
	if (i >= a->length) {
		ERROR_EXIT("Attempted to delete an index outside the current array bounds\n");
	}
	size_t tail = a->length - 1;
	
	if (i != tail) {
		void* src = (char*)a->data + (tail * a->item_size);
		void* dst = (char*)a->data + (i * a->item_size);
		memcpy(dst, src, a->item_size);
	}
	a->length--;
}

void array_free(Array* a) {
	if (a->data) {
		free(a->data);
		a->data = NULL;
	}
	a->length = 0;
	a->capacity = 0;
	a->item_size = 0;
}

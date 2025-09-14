#ifndef BSP_H
#define BSP_H

#include "../vector.h"
#include "../rng.h"
#include "../../data/array.h"

typedef struct branch Branch;

typedef struct branch {
	Branch* left_child;
	Branch* right_child;
	Vector2f position;
	Vector2f size;
} Branch;

Branch* bsp_init(Vector2f map_position, Vector2f map_size, int splits);
Array bsp_get_leaves(Branch* b);
void bsp_free(Branch* b);

#endif
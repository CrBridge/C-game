#include "bsp.h"

// will first be a consistent height, only varying x and y, but eventually
// it should be varying the heights as well, but it shouldnt split
// on that axis
//	my imp was clearly for a tileset, i need blocks as singular things as each entire
// block will be a mesh. How am I doing that?
//		- the answer is that the end product is me making a root branch, using split
//			and then collecting an array of branches. Each branch has a position and
//			size and this is enough for me to construct game objects :)
//			I should be able to simply give each branch a ranged rng value for height

static void split(Branch* b, int remaining) {
	f64 split_percent = rng_next_double(0.15, 0.85);
	printf("Float value from RNG: {%f}\n", split_percent);
	int is_split_horizontal = b->size.y >= b->size.x;

	if (is_split_horizontal) {
		f32 left_height = b->size.y * split_percent;
		b->left_child = malloc(sizeof(Branch));
		if (!b->left_child) {
			ERROR_EXIT("Failed to allocate memory for Branch!\n");
		}
		b->right_child = malloc(sizeof(Branch));
		if (!b->right_child) {
			ERROR_EXIT("Failed to allocate memory for Branch!\n");
		}
		*b->left_child = (Branch) {
			.position = b->position, 
			.size = (Vector2f) { .x = b->size.x, .y = left_height }, 
			.left_child = NULL, .right_child = NULL
		};
		*b->right_child = (Branch) {
			.position = (Vector2f) { .x = b->position.x, .y = b->position.y + left_height },
			.size = (Vector2f) { .x = b->size.x, .y = b->size.y - left_height },
			.left_child = NULL, .right_child = NULL
		};
	}
	else {
		f32 left_width = b->size.x * split_percent;
		b->left_child = malloc(sizeof(Branch));
		if (!b->left_child) {
			ERROR_EXIT("Failed to allocate memory for Branch!\n");
		}
		b->right_child = malloc(sizeof(Branch));
		if (!b->right_child) {
			ERROR_EXIT("Failed to allocate memory for Branch!\n");
		}
		*b->left_child = (Branch) {
			.position = b->position,
			.size = (Vector2f) {.x = left_width, .y = b->size.y },
			.left_child = NULL, .right_child = NULL
		};
		*b->right_child = (Branch) {
			.position = (Vector2f) {.x = b->position.x + left_width, .y = b->position.y },
			.size = (Vector2f) {.x = b->size.x - left_width, .y = b->size.y },
			.left_child = NULL, .right_child = NULL
		};
	}

	if (remaining > 0) {
		split(b->left_child, remaining - 1);
		split(b->right_child, remaining - 1);
	}
}

Branch* bsp_init(Vector2f map_position, Vector2f map_size, int splits) {
	Branch* b = malloc(sizeof(Branch));

	if (!b) {
		ERROR_EXIT("Failed to allocate memory for Branch!\n");
	}

	b->position = map_position;
	b->size = map_size;
	b->left_child = NULL;
	b->right_child = NULL;

	split(b, splits - 1);

	return b;
}

Array bsp_get_leaves(Branch* b) {
	Array leaves = array_init(sizeof(Branch));

	if (b == NULL) return leaves;

	if (!b->left_child && !b->right_child) {
		array_append(&leaves, b);
		return leaves;
	}
	else {
		Array left_leaves = bsp_get_leaves(b->left_child);
		Array right_leaves = bsp_get_leaves(b->right_child);

		for (int i = 0; i < right_leaves.length; i++) {
			Branch* a = (Branch*)right_leaves.data;
			array_append(&left_leaves, &a[i]);
		}
		return left_leaves;
	}
}

void bsp_free(Branch* b) {
	if (!b) return;
	bsp_free(b->left_child);
	bsp_free(b->right_child);
	free(b);
}

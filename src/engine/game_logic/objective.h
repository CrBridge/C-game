#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include "../game_object.h"
#include "player.h"

#include "../math/cube.h"

typedef struct objective_info {
	int is_collected;
} ObjectiveInfo;

void objective_init(GameObject* g);
void objective_update(GameObject* g, f32 dt);
void objective_draw(GameObject* g);

#endif
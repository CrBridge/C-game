// going to store larger state in here
// thats not really much

// I will store a value to represent the current Scene
// maybe functions to handle each scene so I dont need a struct
// macros for constants to take out harcoded values

#ifndef GAME_INFO_H
#define GAME_INFO_H

#define TERRAIN_SIZE 256

#include "game_loader.h"

#include "player.h"
#include "objective.h"
#include "../data/array.h"
#include "../game_object.h"
#include "../rendering/renderer.h"
#include "../math/rng.h"
#include "../camera.h"

typedef enum {
	SCENE_MAIN_MENU,
	SCENE_GAME,
	SCENE_GAME_OVER,
	SCENE_WIN
} Scene;

void game_init(Array* game_objects);

void scene_input(Array* game_objects, f32 dt);
void scene_update(Array* game_objects, f32 dt);
void scene_draw(Array* game_objects);

#endif
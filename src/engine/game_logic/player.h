#ifndef PLAYER_H
#define PLAYER_H

#include "../types.h"
#include "../game_object.h"
#include "../components/transform.h"
#include "../input/input.h"

#include <linmath.h>
#include <FastNoiseLite.h>

typedef struct player_info {
	f32 thrust;
	fnl_state* noise;
} PlayerInfo;

// signatures for the functions that the player GameObject will point to

void player_info_init(GameObject* g, fnl_state* n);

void player_input(GameObject* g, f32 dt);
void player_update(GameObject* g, f32 dt);
void player_draw(GameObject* g);

// cheat functions to easily get player info
// used in collisions, UI etc.
Vector3f player_get_position();
f32 player_get_thrust();
int player_has_player_crashed();

#endif
#ifndef PLAYER_H
#define PLAYER_H

#include "../types.h"
#include "../game_object.h"
#include "../components/transform.h"
#include "../input/input.h"

#include <linmath.h>

// the gameobject could store the player specific information with a void pointer. That way
// we can have unique data for every single game object. For player another approach would be
// to store some static info here, but at that point why am I bothering with GameObject?

// this will be stored in a GameObject using a void pointer, the update function will cast that
// void pointer to this info struct to do things
typedef struct player_info {
	f32 yaw;
	f32 pitch;
} PlayerInfo;

// signatures for the functions that the player GameObject will point to

PlayerInfo player_info_init();

void player_input(GameObject* g, f32 dt);
void player_update(GameObject* g, f32 dt);

#endif
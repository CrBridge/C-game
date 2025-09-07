#include "player.h"

PlayerInfo player_info_init() {
	//
}

void player_input(GameObject* g, f32 dt) {
	if (input_is_key_down(SDL_SCANCODE_Q)) {
		g->transform.rotation[1] += dt * 40.0f;
	}
	if (input_is_key_down(SDL_SCANCODE_E)) {
		g->transform.rotation[1] -= dt * 40.0f;
	}
	if (input_is_key_down(SDL_SCANCODE_W)) {
		g->transform.rotation[0] += dt * 40.0f;
	}
	if (input_is_key_down(SDL_SCANCODE_S)) {
		g->transform.rotation[0] -= dt * 40.0f;
	}
}

void player_update(GameObject* g, f32 dt) {
	vec3 forward;
	component_transform_get_local_forwards(forward, &g->transform);
	vec3_norm(forward, forward);
	vec3_scale(forward, forward, dt * 5.0f);
	vec3_sub(g->transform.position, g->transform.position, forward);
}

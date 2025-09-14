#include "player.h"

static Vector3f player_pos = { 0 };

PlayerInfo player_info_init() {
	PlayerInfo info = { 0 };
	info.pitch = 0.0f;
	info.yaw = 0.0f;
}

void player_input(GameObject* g, f32 dt) {
	// I suppose the end-product would be having player_info store
	// all the needed info for yaw/pitch/roll, then similar to before
	// the inputs can simply update those values. Then at the end I can
	// use quat math to calculate a new rotation matrix
	float angle = dt * 60.0f;

	mat4x4 rot;
	mat4x4_identity(rot);

	if (input_is_key_down(SDL_SCANCODE_Q)) {
		mat4x4_rotate(rot, rot, 0, 1, 0, degree_to_rad(angle));
	}
	if (input_is_key_down(SDL_SCANCODE_E)) {
		mat4x4_rotate(rot, rot, 0, 1, 0, degree_to_rad(-angle));
	}
	if (input_is_key_down(SDL_SCANCODE_W)) {
		mat4x4_rotate(rot, rot, 1, 0, 0, degree_to_rad(-angle));
	}
	if (input_is_key_down(SDL_SCANCODE_S)) {
		mat4x4_rotate(rot, rot, 1, 0, 0, degree_to_rad(angle));
	}
	if (input_is_key_down(SDL_SCANCODE_A)) {
		mat4x4_rotate(rot, rot, 0, 0, 1, degree_to_rad(angle));
	}
	if (input_is_key_down(SDL_SCANCODE_D)) {
		mat4x4_rotate(rot, rot, 0, 0, 1, degree_to_rad(-angle));
	}

	mat4x4_mul(g->transform.rotation, g->transform.rotation, rot);
}

void player_update(GameObject* g, f32 dt) {
	vec3 forward;
	component_transform_get_local_forwards(forward, &g->transform);
	vec3_norm(forward, forward);
	vec3_scale(forward, forward, dt * 5.0f);
	vec3_sub(g->transform.position, g->transform.position, forward);

	// temp!
	player_pos.x = g->transform.position[0];
	player_pos.y = g->transform.position[1];
	player_pos.z = g->transform.position[2];
}

void player_draw(GameObject* g) {
	printf("There should be no more Ship now!\n");
}

Vector3f player_get_position() {
	return player_pos;
}
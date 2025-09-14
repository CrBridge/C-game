#include "player.h"

static Vector3f player_pos = { 0 };
static f32 player_thrust = 0.0f;
static int player_has_crashed = 0;

void player_info_init(GameObject* g, fnl_state* n) {
	g->additional_data = malloc(sizeof(PlayerInfo));
	if (!g->additional_data) {
		ERROR_EXIT("Failed to allocate memory for a game object!\n");
	}
	PlayerInfo* p = (PlayerInfo*)g->additional_data;
	p->thrust = 1.0f;
	player_pos = (Vector3f) { 0 };
	p->noise = n;
	player_has_crashed = 0;
}

void player_input(GameObject* g, f32 dt) {
	// I suppose the end-product would be having player_info store
	// all the needed info for yaw/pitch/roll, then similar to before
	// the inputs can simply update those values. Then at the end I can
	// use quat math to calculate a new rotation matrix
	PlayerInfo* info = (PlayerInfo*)g->additional_data;

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
		mat4x4_rotate(rot, rot, 0, 0, 1, 1.5f * degree_to_rad(angle));
	}
	if (input_is_key_down(SDL_SCANCODE_D)) {
		mat4x4_rotate(rot, rot, 0, 0, 1, 1.5f * degree_to_rad(-angle));
	}
	if (input_is_key_down(SDL_SCANCODE_SPACE)) {
		info->thrust += 0.4f * dt;
	}
	if (input_is_key_down(SDL_SCANCODE_LSHIFT)) {
		info->thrust -= 0.4f * dt;
	}

	float_clamp(&info->thrust, 1.0f, 3.0f);
	//float_clamp(&g->transform.position[1], 0.0f, 2.0f);

	mat4x4_mul(g->transform.rotation, g->transform.rotation, rot);
}

void player_update(GameObject* g, f32 dt) {
	PlayerInfo* info = (PlayerInfo*)g->additional_data;

	vec3 forward;
	component_transform_get_local_forwards(forward, &g->transform);
	vec3_norm(forward, forward);
	vec3_scale(forward, forward, dt * 5.0f * info->thrust);
	vec3_sub(g->transform.position, g->transform.position, forward);

	// temp!
	player_pos.x = g->transform.position[0];
	player_pos.y = g->transform.position[1];
	player_pos.z = g->transform.position[2];
	player_thrust = info->thrust;

	// 256 here comes from the width and height passed into the heighmap load function
	float terrain_height_at_player = fnlGetNoise2D(info->noise, g->transform.position[0] + 256 / 2.0f, g->transform.position[2] + 256 / 2.0f);
	if (g->transform.position[1] < terrain_height_at_player) {
		//printf("Player has collided with the terrain! :( \n\n");
		player_has_crashed = 1;
	}
}

void player_draw(GameObject* g) {
	printf("There should be no more Ship now!\n");
}

Vector3f player_get_position() {
	return player_pos;
}

f32 player_get_thrust() {
	return player_thrust;
}

int player_has_player_crashed() 
{
	return player_has_crashed;
}
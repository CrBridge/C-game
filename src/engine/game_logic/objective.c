#include "objective.h"

void objective_init(GameObject* g) {
	// gonna have to free this eventually
	g->additional_data = malloc(sizeof(ObjectiveInfo));
	if (!g->additional_data) {
		ERROR_EXIT("Failed to allocate memory for a game object!\n");
	}
	ObjectiveInfo* o = (ObjectiveInfo*)g->additional_data;
	o->is_collected = 0;
}

void objective_update(GameObject* g, f32 dt) {
	Vector3f player_pos = player_get_position();

	//Cube collection_boundary = cube_init(
	//	g->transform.position[0] - 5.0f,
	//	g->transform.position[1],
	//	g->transform.position[2] - 5.0f,
	//	5.0f, 5.0f, 5.0f
	//);
	//
	//// TODO! collision is super janky, I need real 3d collision shapes
	//if (cube_is_vector3f_intersecting(collection_boundary, player_pos)) {
	//	((ObjectiveInfo*)g->additional_data)->is_collected = 1;
	//}
	float dx = player_pos.x - g->transform.position.x;
	float dy = player_pos.y - g->transform.position.y;
	float dz = player_pos.z - g->transform.position.z;
	float dist_sq = dx * dx + dy * dy + dz * dz;
	float radius = 2.5f * g->transform.scale; // tune to fit your mesh
	if (dist_sq <= radius * radius) {
		((ObjectiveInfo*)g->additional_data)->is_collected = 1;
	}
}

void objective_draw(GameObject* g) {
	//
}
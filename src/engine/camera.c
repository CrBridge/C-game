#include "camera.h"

#include <string.h>
#include <stdio.h>

const f32 YAW = -90.0f;
const f32 PITCH = 0.0f;
const f32 SPEED = 2.5f;
const f32 SENSITIVITY = 0.1f;
const f32 ZOOM = 45.0f;

// this needs to update on camera rotation (e.g. mouse movement)
// it does not need to be called on position change (e.g. WASD)
void camera_update_vectors(Camera* c) {
	f32 front_x = cos(degree_to_rad(c->yaw)) * cos(degree_to_rad(c->pitch));
	f32 front_y = sin(degree_to_rad(c->pitch));
	f32 front_z = sin(degree_to_rad(c->yaw)) * cos(degree_to_rad(c->pitch));
	vec3_norm(c->front, (vec3) { front_x, front_y, front_z });

	vec3 r_cross;
	vec3_mul_cross(r_cross, c->front, c->world_up);
	vec3_norm(c->right, r_cross);

	vec3 u_cross;
	vec3_mul_cross(u_cross, c->right, c->front);
	vec3_norm(c->up, u_cross);
}

void camera_init(Camera* c) {
	memcpy(c->position, (vec3) { 0, 0, 1 }, sizeof(vec3));
	memcpy(c->front, (vec3) { 0, 0, -1 }, sizeof(vec3));
	memcpy(c->up, (vec3) { 0, 1, 0 }, sizeof(vec3));
	memcpy(c->world_up, c->up, sizeof(vec3));
	c->yaw = YAW;
	c->pitch = PITCH;
	c->move_speed = SPEED;
	c->sensitivity = SENSITIVITY;

	camera_update_vectors(c);
}

void camera_get_view(vec4* view, Camera* c) {
	vec3 target;
	vec3_add(target, c->position, c->front);

	mat4x4_look_at(view, c->position, target, c->up);
}

void camera_move_camera_position(Movement_Direction direction, Camera* c, f32 dt) {
	f32 v = c->move_speed * dt;
	if (direction == FORWARD) {
		vec3 flat_front = { c->front[0], 0.0f, c->front[2] };
		vec3_norm(flat_front, flat_front);

		vec3 v_front;
		vec3_scale(v_front, flat_front, v);
		vec3_add(c->position, c->position, v_front);
	}
	else if (direction == BACKWARD) {
		vec3 flat_front = { c->front[0], 0.0f, c->front[2] };
		vec3_norm(flat_front, flat_front);

		vec3 v_front;
		vec3_scale(v_front, flat_front, v);
		vec3_sub(c->position, c->position, v_front);
	}
	else if (direction == LEFT) {
		vec3 flat_right = { c->right[0], 0.0f, c->right[2] };
		vec3_norm(flat_right, flat_right);

		vec3 v_right;
		vec3_scale(v_right, flat_right, v);
		vec3_sub(c->position, c->position, v_right);
	}
	else if (direction == RIGHT) {
		vec3 flat_right = { c->right[0], 0.0f, c->right[2] };
		vec3_norm(flat_right, flat_right);

		vec3 v_right;
		vec3_scale(v_right, flat_right, v);
		vec3_add(c->position, c->position, v_right);
	}
	else if (direction == UP) {
		vec3 base_up = { 0.0f, 1.0f, 0.0f };

		vec3 v_up;
		vec3_scale(v_up, base_up, v);
		vec3_add(c->position, c->position, v_up);
	}
	else if (direction == DOWN) {
		vec3 base_up = { 0.0f, 1.0f, 0.0f };

		vec3 v_up;
		vec3_scale(v_up, base_up, v);
		vec3_sub(c->position, c->position, v_up);
	}
}

void camera_move_camera_target(f32 dx, f32 dy, Camera* c) {
	dx *= c->sensitivity;
	dy *= c->sensitivity;

	c->yaw += dx;
	c->pitch -= dy;

	float_clamp(&c->pitch, -89.0f, 89.0f);

	camera_update_vectors(c);
}

// tests

void camera_chase_init(ChaseCam* c, GameObject* g, vec3 offset) {
	c->target = g;
	memcpy(c->offset, offset, sizeof(vec3));
}

void camera_get_chase_view(vec4* view, ChaseCam* c) {
	mat4x4 rot;
	mat4x4_identity(rot);
	mat4x4_rotate_X(rot, rot, degree_to_rad(c->target->transform.rotation[0]));
	mat4x4_rotate_Y(rot, rot, degree_to_rad(c->target->transform.rotation[1]));
	mat4x4_rotate_Z(rot, rot, degree_to_rad(c->target->transform.rotation[2]));

	vec4 world_offset;
	vec4 local = { c->offset[0], c->offset[1], c->offset[2], 0.0 };
	mat4x4_mul_vec4(world_offset, rot, local);

	vec3_add(c->position, c->target->transform.position, world_offset);

	vec4 forward = { 0, 0, -1, 0 };
	vec4 world_forward;
	mat4x4_mul_vec4(world_forward, rot, forward);

	vec3 look_at;
	vec3_add(look_at, c->target->transform.position, world_forward);

	mat4x4_look_at(view, c->position, look_at, (vec3) { 0, 1, 0 });
}

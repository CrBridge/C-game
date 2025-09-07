#ifndef CAMERA_H
#define CAMERA_H

#include <linmath.h>

#include "types.h"
#include "math_util.h"

typedef enum Movement_Direction {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
}Movement_Direction;

// standard spectator-style camera
typedef struct camera {
	vec3 position;
	vec3 front;
	vec3 up;
	vec3 right;
	vec3 world_up;
	f32 yaw;
	f32 pitch;
	f32 move_speed;
	f32 sensitivity;
} Camera;

void camera_update_vectors(Camera* c);
void camera_init(Camera* c);
void camera_get_view(vec4* view, Camera* c);
void camera_move_camera_position(Movement_Direction direction, Camera* c, f32 dt);
void camera_move_camera_target(f32 dx, f32 dy, Camera* c);

#include "game_object.h"

// chase-style camera
typedef struct chase_camera {
	GameObject* target;
	vec3 offset;
	vec3 position;
} ChaseCam;

void camera_chase_init(ChaseCam* c, GameObject* g, vec3 offset);
void camera_get_chase_view(vec4* view, ChaseCam* c);

#endif
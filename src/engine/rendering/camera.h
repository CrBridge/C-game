#ifndef CAMERA_H
#define CAMERA_H

#include <linmath.h>

#include "../types.h"
#include "../math_util.h"

typedef enum Movement_Direction{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
}Movement_Direction;

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

void update_vectors(Camera* c);
void camera_init(Camera* c);
void get_view(vec4* view, Camera* c);
void move_camera_position(Movement_Direction direction, Camera* c, f32 dt);
void move_camera_target(f32 dx, f32 dy, Camera* c);

// camera struct with projection and view matrix?
// or maybe just the actual values
// xyz, rotation etc. Then each frame I construct it?
// but constructing the projection each time is wasteful,
// as that will rarely change, so maybe I can just store that
// and we'll figure it out later
// todo: what would I need to store to construct the look_at
// each frame?

// look_at takes in the eye, centre and up, all vec3
// up is easy, just use +y
// eye is the camera position, should be simple
// tricky one is eye, how do I know where we are looking?

// the end goal might be something like
//	mat4x4_lookAt(view, cameraPos, vec3_add(cameraPos,cameraFront), cameraUp);

// render_state can store this new struct, so that I dont need to pass it around
// everywhere. This way I should just be able to pass new updated values by calling
// the function in main without having to pass in the matrix

#endif
#include <glad/glad.h>

#include "../global.h"
#include "render.h"
#include "render_internal.h"

static Render_State_Internal state = {0};
// todo! should be part of some larger state
static Camera camera = {0};

void render_init(void) {
	global.render.width = 480;
	global.render.height = 272;
	global.render.window = render_init_window(global.render.width, global.render.height);

	render_init_quad(&state.vao_quad, &state.vbo_quad, &state.ebo_quad);
	render_init_shaders(&state);
	render_init_image_texture(&state.texture_color);

	camera_init(&camera);
}

void render_begin(void) {
	glClearColor(0.08f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void render_end(void) {
	SDL_GL_SwapWindow(global.render.window);
}

void render_quad(vec2 pos, vec2 size, vec4 color) {
	glUseProgram(state.shader_default);

	// model mat
	mat4x4 model;
	mat4x4_identity(model);
	mat4x4_translate(model, pos[0], pos[1], 0);
	mat4x4_scale_aniso(model, model, size[0], size[1], 1);
	//example of using rotation matrix
	//mat4x4_rotate(model, model, 0.0, 1.0, 0.0, M_PI / 4.0f);
	glUniformMatrix4fv(
		glGetUniformLocation(state.shader_default, "model"),
		1,
		GL_FALSE,
		&model[0][0]
	);

	// view mat
	update_vectors(&camera);
	mat4x4 view;
	get_view(view, &camera);
	glUniformMatrix4fv(
		glGetUniformLocation(state.shader_default, "view"),
		1,
		GL_FALSE,
		&view[0][0]
	);

	glUniform4fv(glGetUniformLocation(state.shader_default, "color"), 1, color);

	glBindVertexArray(state.vao_quad);

	glBindTexture(GL_TEXTURE_2D, state.texture_color);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
}

// idea, since camera is global here, we could define a function in this file
// that will alter it with functions defined in camera.h, that way main.c
// can call this function here without needing access to the camera

//TODO: this is so stupid, fix it. Its a pointless middle-man. And camera
// controller code shouldnt really be here in render. Its just because I set
// the camera here
void update_camera_position(Movement_Direction direction, f32 dt) {
	switch (direction) {
		case FORWARD:
			move_camera_position(FORWARD, &camera, dt);
			break;
		case BACKWARD:
			move_camera_position(BACKWARD, &camera, dt);
			break;
		case LEFT:
			move_camera_position(LEFT, &camera, dt);
			break;
		case RIGHT:
			move_camera_position(RIGHT, &camera, dt);
			break;
	}
}

void update_camera_target(f32 dx, f32 dy) {
	move_camera_target(dx, dy, &camera);
}
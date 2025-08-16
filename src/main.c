#include <SDL3/SDL_timer.h>
#include <linmath.h>

#include "engine/window.h"

#include "engine/rendering/vao.h"
#include "engine/rendering/vbo.h"
#include "engine/rendering/ebo.h"
#include "engine/rendering/shader.h"
#include "engine/rendering/texture.h"
#include "engine/rendering/mesh.h"

#include "engine/math_util.h"
#include "engine/input/input.h"

#include "engine/camera.h"

static void poll_input(int* shouldQuit)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			*shouldQuit = 1;
			break;
		case SDL_EVENT_KEY_DOWN:
			if (event.key.key == SDLK_ESCAPE)
			{
				*shouldQuit = 1;
			}
			break;
		}
	}
}

int main(int argc, char** argv)
{
	// Init SDL and OpenGL
	if(!window_init_window(480, 272, "DAEMON")) {
		return -1;
	}

	input_init_keyboard_state();

	// Init game loop variables
	int shouldQuit = 0;
	f32 dt = 0.0f;
	f32 lastFrame = 0.0f;

	// ===================== Matrix Init ===================== //
	mat4x4 model;
	mat4x4_identity(model);

	mat4x4 projection;
	mat4x4_identity(projection);
	mat4x4_perspective(projection, degree_to_rad(45.0f), window_get_aspect(), 0.1f, 100.0f);

	mat4x4 view;
	Camera camera = {0};
	camera_init(&camera);
	camera.position[2] += 3.0f;
	// ======================================================= //

	// ============= Mesh, Shader & Texture Init ============= //
	Mesh mesh = {0};
	mesh_load_from_obj(&mesh, "./res/models/donut.obj");
	
	u32 shader;
	shader = shader_load("./res/shaders/default.vert", "./res/shaders/default.frag");

	u32 texture;
	texture_init(&texture);
	texture_load_texture(&texture, "./res/textures/donut_diffuse.png");
	// ======================================================= //

	while (!shouldQuit) {
		// delta time calculations
		u64 totalMs = SDL_GetTicks();
		f32 totalS = (f32)totalMs / SDL_MS_PER_SECOND;
		dt = totalS - lastFrame;
		lastFrame = totalS;

		// input and event polling
		poll_input(&shouldQuit);
		input_update_current_keyboard_state();
		// input events - must occur before previous state is updated

		// ================== Camera Controller ================== //
		float x, y;
		SDL_GetRelativeMouseState(&x, &y);
		camera_move_camera_target(x, y, &camera);

		if (input_is_key_released(SDL_SCANCODE_SPACE)) {
			shouldQuit = true;
		}
		if (input_is_key_down(SDL_SCANCODE_W)) {
			camera_move_camera_position(FORWARD, &camera, dt);
		}
		if (input_is_key_down(SDL_SCANCODE_A)) {
			camera_move_camera_position(LEFT, &camera, dt);
		}
		if (input_is_key_down(SDL_SCANCODE_S)) {
			camera_move_camera_position(BACKWARD, &camera, dt);
		}
		if (input_is_key_down(SDL_SCANCODE_D)) {
			camera_move_camera_position(RIGHT, &camera, dt);
		}
		// ======================================================= //

		input_update_previous_keyboard_state();

		// example update by rotating the model matrix
		shader_use(&shader); // just in case
		mat4x4_rotate(model, model, 0.7, 1.0, 0.0, dt * (M_PI / 4.0f));
		shader_set_mat4(&shader, "model", &model);

		shader_set_mat4(&shader, "projection", &projection);
		camera_get_view(view, &camera);
		shader_set_mat4(&shader, "view", &view);

		// rendering
		glClearColor(0.08f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader_use(&shader);
		texture_bind(&texture);
		mesh_draw(&mesh);

		vao_unbind();

		window_swap_buffer();
	}

	// optional: cleaning up data
	texture_clean(&texture);
	shader_clean(&shader);
	mesh_clean(&mesh);

	window_clean_window();

	return 0;
}

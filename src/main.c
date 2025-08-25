#include <SDL3/SDL_timer.h>
#include <linmath.h>

#include "engine/window.h"

#include "engine/rendering/shader.h"
#include "engine/rendering/texture.h"
#include "engine/rendering/mesh.h"

#include "engine/math_util.h"
#include "engine/input/input.h"

#include "engine/components/transform_component.h"
#include "engine/components/skybox_component.h"

#include "engine/game_object.h"

#include "engine/camera.h"

#include "engine/data/array.h"

#include "engine/rendering/renderer.h"

#include "engine/rendering/sprite_batch.h"

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
	if(!window_init_window(960, 544, "DAEMON")) {
		return -1;
	}

	renderer_init(480, 272);

	spritebatch_init(480, 272);

	input_init_keyboard_state();

	// Init game loop variables
	int shouldQuit = 0;
	f32 dt = 0.0f;
	f32 lastFrame = 0.0f;
	f32 totalTime = 0.0f;
	u32 currentFrame = 0;

	// ===================== Matrix Init ===================== //

	mat4x4 projection;
	mat4x4_perspective(projection, degree_to_rad(45.0f), window_get_aspect(), 0.1f, 100.0f);
	//mat4x4_ortho(projection, -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 100.0f);

	mat4x4 view;
	Camera camera = {0};
	camera_init(&camera);
	camera.position[2] += 3.0f;

	Rectangle srcTest = {
		.x = 0,
		.y = 0,
		.width = 32,
		.height = 32
	};
	Rectangle dstTest = {
		.x = 0,
		.y = 0,
		.width = 50,
		.height = 50
	};

	// ======================================================= //

	// ============= Mesh, Shader & Texture Init ============= //

	//donut
	GameObject donut;
	game_object_init(&donut);
	mesh_load_from_obj(&donut.mesh, "./res/models/donut.obj");
	texture_load_texture(&donut.texture, "./res/textures/donut_diffuse.png");
	donut.transform.rotation[0] = 180.0f;
	donut.transform.rotation[2] = 45.0f;

	//floor quad
	GameObject floor;
	game_object_init(&floor);
	mesh_load_quad(&floor.mesh);
	texture_load_texture(&floor.texture, "./res/textures/box.png");
	floor.transform.rotation[0] = 270.0f;
	floor.transform.position[1] -= 1.0f;
	floor.transform.scale = 10.0f;

	const char* skybox[6] = {
		"./res/textures/skybox/right.png",
		"./res/textures/skybox/left.png",
		"./res/textures/skybox/top.png",
		"./res/textures/skybox/bottom.png",
		"./res/textures/skybox/front.png",
		"./res/textures/skybox/back.png",
	};
	Skybox sky = skybox_init(skybox);

	shader mainShader;
	mainShader = shader_load("./res/shaders/default.vert", "./res/shaders/default.frag");

	shader blitShader;
	blitShader = shader_load("./res/shaders/blit.vert", "./res/shaders/blit.frag");

	shader controlShader;
	controlShader = shader_load("./res/shaders/control.vert", "./res/shaders/control.frag");

	shader skyShader;
	skyShader = shader_load("./res/shaders/sky.vert", "./res/shaders/sky.frag");

	// ======================================================= //
	int mode = 0;
	// ======================================================= //
	while (!shouldQuit) {
		// delta time calculations
		u64 totalMs = SDL_GetTicks();
		f32 totalS = (f32)totalMs / SDL_MS_PER_SECOND;
		dt = totalS - lastFrame;
		lastFrame = totalS;
		totalTime += totalS;
		currentFrame++;

		// ================ Input & Event Polling ================ //
		poll_input(&shouldQuit);
		input_update_current_keyboard_state();
		// input - must occur before previous state is updated
		float x, y;
		SDL_GetRelativeMouseState(&x, &y);
		camera_move_camera_target(x, y, &camera);

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
		if (input_is_key_down(SDL_SCANCODE_SPACE)) {
			camera_move_camera_position(UP, &camera, dt);
		}
		if (input_is_key_down(SDL_SCANCODE_LSHIFT)) {
			camera_move_camera_position(DOWN, &camera, dt);
		} // moving donut
		if (input_is_key_down(SDL_SCANCODE_UP)) {
			donut.transform.position[1] += 1.0f * dt;
		}
		if (input_is_key_down(SDL_SCANCODE_DOWN)) {
			donut.transform.position[1] -= 1.0f * dt;
		}
		if (input_is_key_down(SDL_SCANCODE_LEFT)) {
			donut.transform.position[0] -= 1.0f * dt;
		}
		if (input_is_key_down(SDL_SCANCODE_RIGHT)) {
			donut.transform.position[0] += 1.0f * dt;
		}
		input_update_previous_keyboard_state();
		// ======================================================= //

		//TODO! having to set the matrices for multiple shaders is annoying, I think
		//	uniform buffer objects solve this?
		mat4x4 model;
		donut.transform.rotation[1] += dt * 20.0f;
		component_transform_calculate_model_matrix(model, &donut.transform);
		camera_get_view(view, &camera);

		renderer_begin_frame();

		// disable depth writing for the skybox render
		glDepthMask(GL_FALSE);
		shader_use(&skyShader);
		shader_set_mat4(&skyShader, "projection", &projection);
		shader_set_mat4(&skyShader, "view", &view);
		shader_set_float(&skyShader, "time", totalS);
		skybox_draw(&sky);

		// re-enable for objects
		glDepthMask(GL_TRUE);
		shader_use(&mainShader);
		shader_set_mat4(&mainShader, "model", &model);
		shader_set_mat4(&mainShader, "projection", &projection);
		shader_set_mat4(&mainShader, "view", &view);
		game_object_draw(&donut);
		component_transform_calculate_model_matrix(model, &floor.transform);
		shader_set_mat4(&mainShader, "model", &model);
		game_object_draw(&floor);

		// UI Stage
		spritebatch_begin(&controlShader);
		spritebatch_draw(dstTest, srcTest, &floor.texture);
		spritebatch_end();

		shader_use(&blitShader);
		shader_set_int(&blitShader, "frame", currentFrame);
		renderer_end_frame();
	}

	// optional: cleaning up data
	game_object_clean(&donut);
	game_object_clean(&floor);

	skybox_clean(&sky);

	shader_clean(&mainShader);
	shader_clean(&blitShader);
	shader_clean(&skyShader);

	spritebatch_clean();
	renderer_clean();
	window_clean_window();

	return 0;
}

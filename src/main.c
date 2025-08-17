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

#include "engine/components/transform_component.h"

#include "engine/rendering/fbo.h"
#include "engine/rendering/rbo.h"

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
	if(!window_init_window(960, 544, "DAEMON")) {
		return -1;
	}

	input_init_keyboard_state();

	// Init game loop variables
	int shouldQuit = 0;
	f32 dt = 0.0f;
	f32 lastFrame = 0.0f;

	// ===================== Matrix Init ===================== //

	Transform transform;
	component_transform_init(&transform);
	transform.rotation[0] = 180.0f;
	transform.rotation[2] = 45.0f;

	mat4x4 projection;
	mat4x4_identity(projection);
	mat4x4_perspective(projection, degree_to_rad(45.0f), window_get_aspect(), 0.1f, 100.0f);

	mat4x4 view;
	Camera camera = {0};
	camera_init(&camera);
	camera.position[2] += 3.0f;

	// ======================================================= //

	// ============= Mesh, Shader & Texture Init ============= //

	//donut
	Mesh mesh = {0};
	mesh_load_from_obj(&mesh, "./res/models/donut.obj");
	texture donutTexture;
	texture_init(&donutTexture);
	texture_load_texture(&donutTexture, "./res/textures/donut_diffuse.png");
	
	//cube for skybox
	Mesh skyCube = { 0 };
	mesh_load_sky_cube(&skyCube);
	texture skyTexture;
	texture_init(&skyTexture);
	const char* skybox[6] = {
		"./res/textures/skybox/right.png",
		"./res/textures/skybox/left.png",
		"./res/textures/skybox/top.png",
		"./res/textures/skybox/bottom.png",
		"./res/textures/skybox/front.png",
		"./res/textures/skybox/back.png",
	};
	texture_load_cube_texture(&skyTexture, skybox);

	shader mainShader;
	mainShader = shader_load("./res/shaders/default.vert", "./res/shaders/default.frag");

	shader blitShader;
	blitShader = shader_load("./res/shaders/blit.vert", "./res/shaders/blit.frag");

	shader skyShader;
	skyShader = shader_load("./res/shaders/sky.vert", "./res/shaders/sky.frag");

	// ======================================================= //

	// ================== Framebuffer Init =================== //
	
	fbo fbo;
	fbo_init(&fbo);
	fbo_bind(&fbo);
	texture colorAttach;
	fbo_add_buffer(&colorAttach, 480, 272);

	rbo rbo;
	rbo_init(&rbo);
	rbo_attach_depth_stencil_buffers(&rbo, 480, 272);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		ERROR_EXIT("Error on initalising the Framebuffer\n");
	}
	fbo_unbind();

	Mesh screenQuad = {0};
	mesh_load_screen_quad(&screenQuad);

	// ======================================================= //
	while (!shouldQuit) {
		// delta time calculations
		u64 totalMs = SDL_GetTicks();
		f32 totalS = (f32)totalMs / SDL_MS_PER_SECOND;
		dt = totalS - lastFrame;
		lastFrame = totalS;

		// ================ Input & Event Polling ================ //
		poll_input(&shouldQuit);
		input_update_current_keyboard_state();
		// input - must occur before previous state is updated
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
		input_update_previous_keyboard_state();
		// ======================================================= //

		//TODO! having to set the matrices for multiple shaders is annoying, I think
		//	uniform buffer objects solve this?
		shader_use(&mainShader); // just in case
		mat4x4 model;
		transform.rotation[1] += dt * 20.0f;
		component_transform_calculate_model_matrix(model, &transform);
		camera_get_view(view, &camera);

		fbo_bind(&fbo);
		glViewport(0, 0, 480, 272);

		// enable depth testing when rendering the actual screen
		glEnable(GL_DEPTH_TEST);

		// rendering
		glClearColor(0.08f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// disable depth writing for the skybox render
		glDepthMask(GL_FALSE);
		shader_use(&skyShader);
		shader_set_mat4(&skyShader, "projection", &projection);
		shader_set_mat4(&skyShader, "view", &view);
		texture_bind(&skyTexture);
		mesh_draw(&skyCube);

		// re-enable for objects
		glDepthMask(GL_TRUE);
		shader_use(&mainShader);
		shader_set_mat4(&mainShader, "model", &model);
		shader_set_mat4(&mainShader, "projection", &projection);
		shader_set_mat4(&mainShader, "view", &view);
		texture_bind(&donutTexture);
		mesh_draw(&mesh);

		fbo_unbind();
		rbo_unbind();

		glViewport(0, 0, 960, 544);
		// disable depth testing incase it tries clipping the screen quad
		glDisable(GL_DEPTH_TEST);

		shader_use(&blitShader);
		texture_bind(&colorAttach);
		mesh_draw(&screenQuad);

		vao_unbind();

		window_swap_buffer();
	}

	// optional: cleaning up data
	mesh_clean(&mesh);
	mesh_clean(&skyCube);
	mesh_clean(&screenQuad);

	texture_clean(&donutTexture);
	texture_clean(&colorAttach);
	texture_clean_cube(&skyTexture);

	shader_clean(&mainShader);
	shader_clean(&blitShader);
	shader_clean(&skyShader);

	fbo_clean(&fbo);
	rbo_clean(&rbo);

	window_clean_window();

	return 0;
}

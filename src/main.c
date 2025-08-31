#include <SDL3/SDL_timer.h>
#include <linmath.h>

#include "engine/window.h"

#include "engine/rendering/sprite_batch.h"
#include "engine/rendering/renderer.h"

#include "engine/rendering/shader.h"
#include "engine/rendering/texture.h"
#include "engine/rendering/mesh.h"
#include "engine/rendering/font.h"

#include "engine/math/rectangle.h"
#include "engine/math/vector.h"
#include "engine/math_util.h"

#include "engine/input/input.h"

#include "engine/components/transform_component.h"
#include "engine/components/skybox_component.h"

#include "engine/game_object.h"
#include "engine/camera.h"

#include "engine/data/array.h"
#include "engine/data/map.h"

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
	u32 currentFrame = 0;

	// ===================== Matrix Init ===================== //

	mat4x4 projection;
	mat4x4_perspective(projection, degree_to_rad(45.0f), window_get_aspect(), 0.1f, 100.0f);

	mat4x4 view;
	Camera camera = {0};
	camera_init(&camera);
	camera.position[2] += 3.0f;

	// initalising positions for spritebatch drawing
	Rectangle srcTest = {
		.x = 0,
		.y = 0,
		.width = 70,
		.height = 10
	};
	Rectangle dstTest = {
		.x = 0,
		.y = 0,
		.width = 120,
		.height = 12
	};
	Vector2f vecTest = {
		.x = 0,
		.y = 0
	};

	// ======================================================= //

	// ============= Mesh, Shader & Texture Init ============= //

	// game object array
	Array game_objects = array_init(sizeof(GameObject));

	//donut
	GameObject donut;
	game_object_init(&donut);
	mesh_load_from_obj(&donut.mesh, "./res/models/donut.obj");
	texture_load_texture(&donut.texture, "./res/textures/donut_diffuse.png");
	donut.transform.rotation[0] = 180.0f;
	donut.transform.rotation[2] = 45.0f;
	array_append(&game_objects, &donut);

	GameObject ground;
	game_object_init(&ground);
	mesh_load_quad(&ground.mesh);
	texture_load_from_color(&ground.texture, (u8[4]) { 50, 130, 10, 255 });
	ground.transform.rotation[0] = 270.0f;
	ground.transform.scale = 20.0f;
	ground.transform.position[1] = -1.0f;

	// skybox
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

	shader shellShader;
	shellShader = shader_load("./res/shaders/shell.vert", "./res/shaders/shell.frag");

	shader blitShader;
	blitShader = shader_load("./res/shaders/blit.vert", "./res/shaders/blit.frag");

	shader controlShader;
	controlShader = shader_load("./res/shaders/control.vert", "./res/shaders/control.frag");

	shader skyShader;
	skyShader = shader_load("./res/shaders/sky.vert", "./res/shaders/sky.frag");

	// ================== Font & Sprite Init ================== //
	
	//TODO! would be better if the font_init just loaded the texture too
	Texture font_tex;
	texture_init(&font_tex);
	texture_load_texture(&font_tex, "./res/textures/font-testing.png");
	Font font = font_init("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!?.,/-%()[]\":#", &font_tex, 10, 10);
	Texture bg;
	texture_init(&bg);
	texture_load_from_color(&bg, (u8[4]) { 255, 255, 255, 255 });

	// ======================================================= //
	while (!shouldQuit) {
		// delta time calculations
		u64 totalMs = SDL_GetTicks();
		f32 totalS = (f32)totalMs / SDL_MS_PER_SECOND;
		dt = totalS - lastFrame;
		lastFrame = totalS;
		currentFrame++;
		// ================ Input & Event Polling ================ //
		poll_input(&shouldQuit);
		input_update_current_keyboard_state();
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
		}
		input_update_previous_keyboard_state();
		// ======================================================= //

		mat4x4 model;
		((GameObject*)game_objects.data)[0].transform.rotation[1] += dt * 20.0f;

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
		shader_set_mat4(&mainShader, "projection", &projection);
		shader_set_mat4(&mainShader, "view", &view);
		shader_set_float(&mainShader, "time", totalS);
		for (int i = 0; i < game_objects.length; i++) {
			GameObject* g = (GameObject*)game_objects.data;
			component_transform_calculate_model_matrix(model, &g[i].transform);
			shader_set_mat4(&mainShader, "model", &model);
			game_object_draw(&g[i]);
		}

		// shell texturing, need more work until I can put objects with
		// unique render systems into the objects array
		shader_use(&shellShader);
		shader_set_mat4(&shellShader, "projection", &projection);
		shader_set_mat4(&shellShader, "view", &view);
		f32 threshold = 0.0f;
		for (int i = 0; i < 16; i++) {
			component_transform_calculate_model_matrix(model, &ground.transform);
			shader_set_mat4(&shellShader, "model", &model);
			shader_set_float(&shellShader, "threshold", threshold);
			game_object_draw(&ground);
			threshold += 0.1f;
			ground.transform.position[1] += 0.025f;
		}
		ground.transform.position[1] = -1.0f;

		// UI Stage
		glDisable(GL_DEPTH_TEST);
		spritebatch_begin(&controlShader);
		spritebatch_draw(dstTest, srcTest, &bg, (Vector3f){0, 0, 1.0});
		//spritebatch_draw_string(vecTest, &font, "HELLO WORLD!", 1.0f, (Vector3f){1,1,1});
		spritebatch_draw_string(vecTest, &font, "WHATS GOING ON?", 1.0f, (Vector3f) { 1, 1, 1 });
		spritebatch_end();

		// scale resolution / blit to screen
		shader_use(&blitShader);
		shader_set_int(&blitShader, "frame", currentFrame);
		renderer_end_frame();
	}

	// optional: cleaning up data
	game_object_clean(&donut);
	game_object_clean(&ground);

	skybox_clean(&sky);

	shader_clean(&mainShader);
	shader_clean(&blitShader);
	shader_clean(&skyShader);

	font_clean(&font);
	texture_clean(&bg);

	spritebatch_clean();
	renderer_clean();
	window_clean_window();

	return 0;
}

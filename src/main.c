#include <SDL3/SDL_timer.h>
#include <linmath.h>

#include "engine/window.h"
#include "engine/rendering/renderer.h"
#include "engine/math_util.h"
#include "engine/input/input.h"
#include "engine/components/skybox.h"
#include "engine/game_object.h"
#include "engine/camera.h"
#include "engine/data/array.h"
#include "engine/math/rng.h"
#include "engine/game_logic/game_loader.h"
#include "engine/game_logic/game_info.h"

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
	if(!window_init_window(960, 544, "plane game")) {
		return -1;
	}
	renderer_init(480, 272); // also initialises spritebatch

	input_init_keyboard_state();

	// Init game loop variables
	int shouldQuit = 0;
	f32 dt = 0.0f;
	f32 lastFrame = 0.0f;
	u32 currentFrame = 0;

	// ================ RNG & Game Info Init ================= //
	rng_set_seed(0);
	game_global_init();
	// ======================================================= //

	// game object array
	Array game_objects = array_init(sizeof(GameObject));
	game_init(&game_objects);

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

	// ===================== Matrix Init ===================== //
	mat4x4 projection;
	mat4x4_perspective(projection, degree_to_rad(45.0f), window_get_aspect(), 0.1f, 10000.0f);

	mat4x4 view;
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

		scene_input(&game_objects, dt);

		input_update_previous_keyboard_state();
		// ======================================================= //
		scene_update(&game_objects, dt);

		camera_get_chase_view(view);

		renderer_begin_frame(&projection, &view, totalS, currentFrame);

		renderer_draw_skybox(&sky); // stays here as its used for all scenes
		scene_draw(&game_objects);

		// scale resolution / blit to screen / post-process
		renderer_end_frame();
	}

	for (int i = 0; i < game_objects.length; i++) {
		GameObject* g = game_objects.data;
		game_object_clean(&g[i]);
	}
	array_free(&game_objects);

	skybox_clean(&sky);

	game_assets_unload();

	spritebatch_clean();
	renderer_clean();
	window_clean_window();

	return 0;
}

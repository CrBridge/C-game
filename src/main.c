#include <SDL3/SDL_timer.h>
#include <linmath.h>

#include "engine/window.h"

#include "engine/rendering/sprite_batch.h"
#include "engine/rendering/renderer.h"

#include "engine/rendering/texture.h"
#include "engine/rendering/mesh.h"
#include "engine/rendering/font.h"

#include "engine/math/rectangle.h"
#include "engine/math/vector.h"
#include "engine/math_util.h"

#include "engine/input/input.h"

#include "engine/components/transform.h"
#include "engine/components/skybox.h"

#include "engine/game_object.h"
#include "engine/camera.h"

#include "engine/data/array.h"
#include "engine/data/map.h"

#include "engine/game_logic/player.h"

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
		// TODO: support resizing?
		//  would need to either change how i get window_width/height
		//	or update that on resize. I think thats about it though,
		// renderer only gets the values via window_get_width/height
		// so as long as that handles it we should be good
		// suppose I'd also want to reinit the projection as it uses
		// aspect ratio...
	}
}

int main(int argc, char** argv)
{
	// Init SDL and OpenGL
	if(!window_init_window(960, 544, "DAEMON")) {
		return -1;
	}
	renderer_init(480, 272); // also initialises spritebatch

	input_init_keyboard_state();

	// Init game loop variables
	int shouldQuit = 0;
	f32 dt = 0.0f;
	f32 lastFrame = 0.0f;
	u32 currentFrame = 0;

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

	// TODO! support some kind of global resource handling so I don't load duplicates

	// game object array
	Array game_objects = array_init(sizeof(GameObject));

	// tracking this for collision, how can I use this in an objects update function though?
	fnl_state noise = fnlCreateState();
	noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
	noise.frequency = 0.1f;
	//noise.octaves = 10;

	{
		// terrain
		GameObject terrain;
		game_object_init(&terrain, RENDER_TERRAIN);
		mesh_load_from_heightmap(&terrain.mesh, &noise, 128, 128);
		//texture_load_from_color(&terrain.texture, (u8[4]) { 255, 255, 255, 255 });
		texture_load_texture(&terrain.texture, "./res/textures/terrain_dither.png");
		array_append(&game_objects, &terrain);

		// player
		GameObject ship;
		game_object_init(&ship, RENDER_DEFAULT);
		mesh_load_cube(&ship.mesh);
		texture_load_texture(&ship.texture, "./res/textures/box.png");
		ship.transform.position[1] += 5.0f;
		ship.input = player_input;
		ship.update = player_update;
		array_append(&game_objects, &ship);
	}

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
	// ================== Font & Sprite Init ================== //
	
	//TODO! would be better if the font_init just loaded the texture too
	Texture font_tex;
	texture_init(&font_tex);
	texture_load_texture(&font_tex, "./res/textures/font-testing.png");
	Font font = font_init("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!?.,/-%()[]\":#", &font_tex, 10, 10);
	Texture bg;
	texture_init(&bg);
	texture_load_from_color(&bg, (u8[4]) { 255, 255, 255, 255 });
	Texture overlay;
	texture_init(&overlay);
	texture_load_texture(&overlay, "./res/textures/overlay.png");

	// ===================== Matrix Init ===================== //
	mat4x4 projection;
	mat4x4_perspective(projection, degree_to_rad(45.0f), window_get_aspect(), 0.1f, 100.0f);

	mat4x4 view;
	ChaseCam camera;
	camera_chase_init(&camera, &((GameObject*)game_objects.data)[1], (vec3) { 0.f, 0.f, 5.f });
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

		for (int i = 0; i < game_objects.length; i++) {
			GameObject* g = &((GameObject*)game_objects.data)[i];
			if (g->input) g->input(g, dt);
		}

		input_update_previous_keyboard_state();
		// ======================================================= //

		for (int i = 0; i < game_objects.length; i++) {
			GameObject* g = &((GameObject*)game_objects.data)[i];
			if (g->update) g->update(g, dt);
		}

		// Lots of ways to go about that, right now I'd lean towards implementing an event system,
		// I suppose this would work by having a globaly accessible event queue
		// would this even solve my problem?

		// TODO! example of checking for terrain collision. This sucks, objects need
		//	a better way of interacting with other objects
		//GameObject g = ((GameObject*)game_objects.data)[1];
		//float height_of_collision = fnlGetNoise2D(&noise, g.transform.position[0], g.transform.position[2]);
		//if (g.transform.position[1] <= height_of_collision) {
		//	printf("Collision Detected!\n");
		//}

		camera_get_chase_view(view, &camera);

		renderer_begin_frame(&projection, &view, totalS, currentFrame);

		renderer_draw_skybox(&sky);
		for (int i = 0; i < game_objects.length; i++) {
			GameObject* g = (GameObject*)game_objects.data;
			renderer_draw_game_object(&g[i]);
		}

		// UI Stage
		spritebatch_begin();
		spritebatch_draw_vec_dst((Vector2f){0,0}, &overlay, (Vector3f){1,1,1});
		spritebatch_draw(dstTest, srcTest, &bg, (Vector3f){0, 0, 1.0});
		spritebatch_draw_string(vecTest, &font, "HELLO WORLD!", 1.0f, (Vector3f){1,1,1});
		spritebatch_end();

		// scale resolution / blit to screen
		renderer_end_frame();
	}

	// optional - cleaning up data
	for (int i = 0; i < game_objects.length; i++) {
		GameObject* g = game_objects.data;
		game_object_clean(&g[i]);
	}
	array_free(&game_objects);

	skybox_clean(&sky);

	font_clean(&font);
	texture_clean(&bg);

	spritebatch_clean();
	renderer_clean();
	window_clean_window();

	return 0;
}

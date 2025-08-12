#include <SDL3/SDL_timer.h>

#include "engine/global.h"
#include "engine/input.h"

int main(int argc, char** argv) {
	render_init();

	bool should_quit = false;
	
	f32 dt = 0.0f;
	f32 last_frame = 0.0f;

	init_keyboard_state();

	while (!should_quit) {
		u64 total_ms = SDL_GetTicks();
		f32 total_s = (f32)total_ms / SDL_MS_PER_SECOND;
		dt = total_s - last_frame;
		last_frame = total_s;

		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_EVENT_QUIT:
					should_quit = true;
					break;
				default:
					break;
			}
		}

		update_current_keyboard_state();

		if (is_key_pressed(SDL_SCANCODE_ESCAPE))
		{
			should_quit = true;
		}

		if (is_key_down(SDL_SCANCODE_W)) {
			update_camera_position(FORWARD, dt);
		}
		if (is_key_down(SDL_SCANCODE_A)) {
			update_camera_position(LEFT, dt);
		}
		if (is_key_down(SDL_SCANCODE_S)) {
			update_camera_position(BACKWARD, dt);
		}
		if (is_key_down(SDL_SCANCODE_D)) {
			update_camera_position(RIGHT, dt);
		}

		// this works for giving relative positions, but I'm
		// sure I'd want absolute too at some point, I suppose
		// it works for now
		float x, y;
		SDL_GetRelativeMouseState(&x, &y);
		update_camera_target(x, y);

		render_begin();

		render_quad(
			(vec2) {0, 0},
			(vec2) {1, 1},
			(vec4) {1, 1, 1, 1}
		);
		
		render_end();

		update_previous_keyboard_state();
	}

	return 0;
}
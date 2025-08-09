#include "engine/global.h"

int main(int argc, char** argv) {
	render_init();

	bool should_quit = false;

	while (!should_quit) {
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

		// nicer input example
		const bool* keyboard = SDL_GetKeyboardState(NULL);
		if (keyboard[SDL_SCANCODE_ESCAPE])
		{
			should_quit = true;
		}

		render_begin();

		render_quad(
			(vec2) {0, 0},
			(vec2) {1, 1},
			(vec4) {1, 1, 1, 1}
		);
		
		render_end();
	}

	return 0;
}
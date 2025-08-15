#include <SDL3/SDL_timer.h>
#include <linmath.h>

#include "engine/window.h"

#include "engine/rendering/vao.h"
#include "engine/rendering/vbo.h"
#include "engine/rendering/ebo.h"
#include "engine/rendering/shader.h"
#include "engine/rendering/texture.h"

#include "engine/math_util.h"
#include "engine/input/input.h"

void poll_input(int* shouldQuit)
{
	SDL_Event event;
	SDL_PollEvent(&event);

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

int main(int argc, char** argv)
{
	// Init SDL and OpenGL
	if(!init_window(480, 272, "DAEMON")) {
		return -1;
	}

	input_init_keyboard_state();

	// Init game loop variables
	int shouldQuit = 0;
	f32 dt = 0.0f;
	f32 lastFrame = 0.0f;

	// testing mats
	mat4x4 model;
	mat4x4_identity(model);

	Vertex vertices[] = {
		{{-0.5, -0.5,  0.5}, {0, 0, 1}, {0, 0}},
		{{ 0.5, -0.5,  0.5}, {0, 0, 1}, {1, 0}},
		{{ 0.5,  0.5,  0.5}, {0, 0, 1}, {1, 1}},
		{{-0.5,  0.5,  0.5}, {0, 0, 1}, {0, 1}},

		{{ 0.5, -0.5, -0.5}, {0, 0, -1}, {0, 0}},
		{{-0.5, -0.5, -0.5}, {0, 0, -1}, {1, 0}},
		{{-0.5,  0.5, -0.5}, {0, 0, -1}, {1, 1}},
		{{ 0.5,  0.5, -0.5}, {0, 0, -1}, {0, 1}},

		{{-0.5, -0.5, -0.5}, {-1, 0, 0}, {0, 0}},
		{{-0.5, -0.5,  0.5}, {-1, 0, 0}, {1, 0}},
		{{-0.5,  0.5,  0.5}, {-1, 0, 0}, {1, 1}},
		{{-0.5,  0.5, -0.5}, {-1, 0, 0}, {0, 1}},

		{{ 0.5, -0.5,  0.5}, {1, 0, 0}, {0, 0}},
		{{ 0.5, -0.5, -0.5}, {1, 0, 0}, {1, 0}},
		{{ 0.5,  0.5, -0.5}, {1, 0, 0}, {1, 1}},
		{{ 0.5,  0.5,  0.5}, {1, 0, 0}, {0, 1}},

		{{-0.5,  0.5,  0.5}, {0, 1, 0}, {0, 0}},
		{{ 0.5,  0.5,  0.5}, {0, 1, 0}, {1, 0}},
		{{ 0.5,  0.5, -0.5}, {0, 1, 0}, {1, 1}},
		{{-0.5,  0.5, -0.5}, {0, 1, 0}, {0, 1}},

		{{-0.5, -0.5, -0.5}, {0, -1, 0}, {0, 0}},
		{{ 0.5, -0.5, -0.5}, {0, -1, 0}, {1, 0}},
		{{ 0.5, -0.5,  0.5}, {0, -1, 0}, {1, 1}},
		{{-0.5, -0.5,  0.5}, {0, -1, 0}, {0, 1}},
	};

	u32 indices[] = {
		 0,  1,  2,  2,  3,  0, 
		 4,  5,  6,  6,  7,  4, 
		 8,  9, 10, 10, 11,  8, 
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20 
	};

	u32 vbo, vao, ebo;
	u32 shader;
	u32 texture;

	vbo_init(&vbo);
	vao_init(&vao);
	ebo_init(&ebo);
	
	vao_bind(&vao);

	vbo_bind(&vbo);
	vbo_add_data(&vbo, vertices, sizeof(vertices) / sizeof(vertices[0]));

	ebo_bind(&ebo);
	ebo_add_data(&ebo, indices, sizeof(indices) / sizeof(indices[0]));

	vao_setup_vbo_attrib(&vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
	vao_setup_vbo_attrib(&vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	vao_setup_vbo_attrib(&vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	vao_unbind();

	shader = shader_load("./res/shaders/default.vert", "./res/shaders/default.frag");

	texture_init(&texture);
	texture_load_texture(&texture, "./res/textures/box.png");

	while (!shouldQuit) {
		// delta time calculations
		u64 totalMs = SDL_GetTicks();
		f32 totalS = (f32)totalMs / SDL_MS_PER_SECOND;
		dt = totalS - lastFrame;
		lastFrame = totalS;

		// poll SDL events (replace with better input)
		poll_input(&shouldQuit);
		input_update_current_keyboard_state();
		// input stuff here
		if (input_is_key_released(SDL_SCANCODE_SPACE)) {
			shouldQuit = true;
		}
		input_update_previous_keyboard_state();

		// example update by rotating the model matrix
		shader_use(&shader); // just in case
		mat4x4_rotate(model, model, 0.0, 1.0, 0.5, dt * (M_PI / 4.0f));
		shader_set_mat4(&shader, "model", &model);
		// end of example

		// rendering
		glClearColor(0.5f, 0.3f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader_use(&shader);
		texture_bind(&texture);
		vao_bind(&vao);

		// shouldn't be manually giving index count, calculate it
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

		vao_unbind();

		swap_buffer();
	}

	// optional: cleaning up data
	ebo_clean(&ebo);
	vao_clean(&vao);
	vbo_clean(&vbo);
	texture_clean(&texture);
	shader_clean(&shader);

	clean_window();

	return 0;
}

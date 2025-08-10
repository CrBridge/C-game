#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <stb_image.h>

#include "../util.h"
#include "../global.h"

#include "render.h"
#include "render_internal.h"

SDL_Window* render_init_window(i16 width, i16 height) {
	if (!SDL_Init(SDL_INIT_VIDEO)){
		ERROR_EXIT("Failed to init SDL: %s\n", SDL_GetError());
	}

	// state openGL version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//debug
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	// enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_Window* window = SDL_CreateWindow(
		"Daemon",
		width,
		height,
		SDL_WINDOW_OPENGL
	);

	if (!window) {
		ERROR_EXIT("Failed to create window: %s\n", SDL_GetError());
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (context == NULL) {
		ERROR_EXIT("Failed to create OpenGL context: %s\n", SDL_GetError());
	}

	// set cursor relative to screen
	SDL_SetWindowRelativeMouseMode(window, true);

	// v-sync enable
	SDL_GL_SetSwapInterval(1);

	SDL_GL_MakeCurrent(window, context);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		ERROR_EXIT("Failed to initialize GLAD\n");
	}

	printf("- Shading Language = %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("- Version = %s\n", glGetString(GL_VERSION));
	printf("- Vendor = %s\n", glGetString(GL_VENDOR));
	printf("- Renderer = %s\n\n", glGetString(GL_RENDERER));

	// enable back-face culling and set viewport to window height/width
	// eventually, this should be in some kind of callback to handle the change on
	// window resize. Gotta figure out how to do that with SDL (event poll probably)
	glViewport(0, 0, width, height);
	// Im setting the front faces in clockwise vertex order as thats how I defined the vertices
	// it could be that this is not the norm and models I load in the future need the opposite.
	// I'd imagine theres an easy way (with code or on export) to change this but if not
	// this might need to change
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);

	// enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return window;
}

void render_init_quad(u32* vao, u32* vbo, u32* ebo) {
	// xyz, uv
	f32 vertices[] = {
		0.5, 0.5, -1, 0, 0,
		0.5, -0.5, -1, 0, 1,
		-0.5, -0.5, -1, 1, 1,
		-0.5, 0.5, -1, 1, 0
	};

	u32 indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, vao);
	glGenBuffers(1, vbo);
	glGenBuffers(1, ebo);

	glBindVertexArray(*vao);

	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), NULL);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(3 * sizeof(f32)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void render_init_shaders(Render_State_Internal* state) {
	state->shader_default = render_shader_create("./res/shaders/default.vert", "./res/shaders/default.frag");
	
	//Todo! seems that degree->rad could be degrees * (M_PI/180), this should be defined somewhere
	//Todo! will need to alter this in some instances (e.g. window resize)
	//Todo! Add a view matrix as well, thats a whole camera system, lots of work
	mat4x4_perspective(state->view_projection, degree_to_rad(45.0f), 480.0 / 270.0, 0.1, 100.0);

	glUseProgram(state->shader_default);
	glUniformMatrix4fv(
		glGetUniformLocation(state->shader_default, "projection"),
		1,
		GL_FALSE,
		&state->view_projection[0][0]
	);
}

// easy place to have us load the texture
void render_init_image_texture(u32* texture) {
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	// set sampler parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, channels;
	const char* path = "./res/textures/Gammoth.png";
	unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

	if (!data) {
		//Todo: instead of failing, a fallback texture might be nice
		ERROR_EXIT("Failed to load texture: %s\n", path);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
}

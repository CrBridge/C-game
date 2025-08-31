#include "renderer.h"

//TODO! heres an idea, the struct also stores any values that a shader may use as a uniform
// (except model matrix), then it can also store current bound shader, then I can call a function
// to render something by passing in the game object and the shader I want to use for it. It then
// checks if the shader passed in is different to the current bound one and if it is, we can call
// a function to bind it and set all its uniforms, but how do we know what uniforms to use?
// a solution is to just call all of them but thats a bit annoying. Also, how do we account
// for special systems, e.g. A skybox is not a gameobject, shell texturing requires multiple
// draw calls etc. do those vtable things solve this? Idk but I should look into them they
// sounded interesting

// Simpler system might be just to have game object store an enum for its 'Render System'
// enum could store default, skybox, shells etc. and the game object draw function could use
// a switch statement to handle how this is used

typedef struct renderer {
	int native_width, native_height;
	fbo fbo;
	rbo rbo;
	tex_id color_attachment;
	Mesh screen_quad;
} Renderer;

static Renderer renderState = { 0 };

void renderer_init(int width, int height) {
	renderState.native_width = width;
	renderState.native_height = height;

	fbo_init(&renderState.fbo);
	fbo_bind(&renderState.fbo);
	fbo_add_buffer(&renderState.color_attachment, width, height);

	rbo_init(&renderState.rbo);
	rbo_attach_depth_stencil_buffers(&renderState.rbo, width, height);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		ERROR_EXIT("Error on initalising the Framebuffer\n");
	}

	fbo_unbind();

	mesh_load_screen_quad(&renderState.screen_quad);
}

void renderer_begin_frame() {
	fbo_bind(&renderState.fbo);
	glViewport(0, 0, renderState.native_width, renderState.native_height);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.08f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_end_frame() {
	fbo_unbind();
	rbo_unbind();

	glViewport(0, 0, window_get_width(), window_get_height());
	glDisable(GL_DEPTH_TEST);

	texture_bind(&renderState.color_attachment);
	mesh_draw(&renderState.screen_quad);

	vao_unbind();

	window_swap_buffer();
}

void renderer_clean() {
	mesh_clean(&renderState.screen_quad);
	texture_delete(&renderState.color_attachment);

	fbo_clean(&renderState.fbo);
	rbo_clean(&renderState.rbo);

	renderState.native_width = 0;
	renderState.native_height = 0;
}

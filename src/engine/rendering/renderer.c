#include "renderer.h"

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

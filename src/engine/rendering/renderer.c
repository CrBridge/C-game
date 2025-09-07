#include "renderer.h"

typedef struct {
	mat4x4* projection;
	mat4x4* view;
	u32 time;
	u32 frame;
} UniformInfo;

typedef struct renderer {
	u16 native_width, native_height;
	Mesh screen_quad;
	UniformInfo frame_info;
	fbo fbo;
	rbo rbo;
	tex_id color_attachment;
	shader* bound_shader;
	RenderType bound_shader_identifier;
} Renderer;

static Renderer render_state = { 0 };

// registering shaders
shader main_shader;
shader debug_shader;
shader shell_shader;
shader terrain_shader;
shader blit_shader;
shader sky_shader;

void renderer_init(int width, int height) {
	render_state.native_width = width;
	render_state.native_height = height;

	fbo_init(&render_state.fbo);
	fbo_bind(&render_state.fbo);
	fbo_add_buffer(&render_state.color_attachment, width, height);

	rbo_init(&render_state.rbo);
	rbo_attach_depth_stencil_buffers(&render_state.rbo, width, height);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		ERROR_EXIT("Error on initalising the Framebuffer\n");
	}

	fbo_unbind();

	mesh_load_screen_quad(&render_state.screen_quad);

	// NOTE! shaders that are intended for use with spritebatch will be defined there instead
	main_shader = shader_load("./res/shaders/default.vert", "./res/shaders/default.frag");
	debug_shader = shader_load("./res/shaders/debug.vert", "./res/shaders/debug.frag");
	shell_shader = shader_load("./res/shaders/shell.vert", "./res/shaders/shell.frag");
	terrain_shader = shader_load("./res/shaders/terrain.vert", "./res/shaders/terrain.frag");
	blit_shader = shader_load("./res/shaders/blit.vert", "./res/shaders/blit.frag");
	sky_shader = shader_load("./res/shaders/sky.vert", "./res/shaders/sky.frag");

	spritebatch_init(width, height);
}

void renderer_begin_frame(mat4x4* projection, mat4x4* view, u32 time, u32 frame) {
	fbo_bind(&render_state.fbo);
	glViewport(0, 0, render_state.native_width, render_state.native_height);

	glEnable(GL_DEPTH_TEST);
	//TODO! should be able to configure this
	glClearColor(0.08f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	render_state.frame_info.frame = frame;
	render_state.frame_info.time = time;
	render_state.frame_info.projection = projection;
	render_state.frame_info.view = view;
}

void renderer_use_shader(RenderType shader_type) {
	if (render_state.bound_shader_identifier != shader_type) {
		switch (shader_type) {
			case RENDER_DEFAULT:
				render_state.bound_shader = &main_shader;
				break;
			case RENDER_DEBUG:
				render_state.bound_shader = &debug_shader;
				break;
			case RENDER_SHELL:
				render_state.bound_shader = &shell_shader;
				break;
			case RENDER_TERRAIN:
				render_state.bound_shader = &terrain_shader;
				break;
			case RENDER_SKY:
				render_state.bound_shader = &sky_shader;
				break;
			case RENDER_BLIT:
				render_state.bound_shader = &blit_shader;
				break;
			default:
				ERROR_EXIT("Unknown shader was ID was given to renderer_use_shader\n");
				break;
		}
		render_state.bound_shader_identifier = shader_type;
		shader_use(render_state.bound_shader);
		shader_set_mat4(render_state.bound_shader, "projection", render_state.frame_info.projection);
		shader_set_mat4(render_state.bound_shader, "view", render_state.frame_info.view);
		shader_set_float(render_state.bound_shader, "time", render_state.frame_info.time);
		shader_set_int(render_state.bound_shader, "frame", render_state.frame_info.frame);
	}
}

void renderer_draw_game_object(GameObject* g) {
	renderer_use_shader(g->type);

	// cant declare in switch cases threshold and y_start are 
	// only needed for shell type but thats okay :-)
	f32 y_start = g->transform.position[1];
	f32 threshold = 0.0f;
	mat4x4 model;
	switch (g->type) {
		case RENDER_DEFAULT:
			component_transform_calculate_model_matrix(model, &g->transform);
			shader_set_mat4(render_state.bound_shader, "model", &model);
			game_object_draw(g);
			break;
		// TODO! this GL_LINES isn't really what I want, proper wireframe showing triangles
		//	would need a call to glPolygonMode with GL_LINE, but enabling and disabling it
		//	for every draw call using it seems wasteful. I could set it in the renderer_use_shader,
		//	and disable it once it has switched off of debug, but that would cause issues when going
		//	from the debug shader to spritebatch calls because spritebatch isn't really hooked up
		//	here at all, so thats something I'll have to work on to fix this
		case RENDER_DEBUG:
			component_transform_calculate_model_matrix(model, &g->transform);
			shader_set_mat4(render_state.bound_shader, "model", &model);
			game_object_draw_debug(g, GL_LINES);
			break;
		// hardcoding shells and thresholds, not sure how else to go about it :/
		// supposed the solution would be to store optional niche data with union
		case RENDER_SHELL:
			for (int i = 0; i < 16; i++) {
				component_transform_calculate_model_matrix(model, &g->transform);
				shader_set_mat4(render_state.bound_shader, "model", &model);
				shader_set_float(render_state.bound_shader, "threshold", threshold);
				game_object_draw(g);
				threshold += 0.1f;
				g->transform.position[1] += 0.025f;
			}
			g->transform.position[1] = y_start;
			break;
		case RENDER_TERRAIN:
			component_transform_calculate_model_matrix(model, &g->transform);
			shader_set_mat4(render_state.bound_shader, "model", &model);
			game_object_draw(g);
			break;
		default:
			ERROR_EXIT("Renderer failed to draw a game object. The object either had an undefined \
				type or one which is unsupported (SKY, CONTROL, BLIT)");
			break;
	}
}

void renderer_draw_skybox(Skybox* s) {
	glDepthMask(GL_FALSE);
	renderer_use_shader(RENDER_SKY);

	skybox_draw(s);

	glDepthMask(GL_TRUE);
}

void renderer_end_frame() {
	renderer_use_shader(RENDER_BLIT);

	fbo_unbind();
	rbo_unbind();

	glViewport(0, 0, window_get_width(), window_get_height());
	glDisable(GL_DEPTH_TEST);

	texture_bind(&render_state.color_attachment);
	mesh_draw(&render_state.screen_quad);

	vao_unbind();

	window_swap_buffer();
}

void renderer_clean() {
	shader_clean(&main_shader);
	shader_clean(&shell_shader);
	shader_clean(&sky_shader);
	shader_clean(&blit_shader);

	mesh_clean(&render_state.screen_quad);
	texture_delete(&render_state.color_attachment);

	fbo_clean(&render_state.fbo);
	rbo_clean(&render_state.rbo);

	render_state.native_width = 0;
	render_state.native_height = 0;
}

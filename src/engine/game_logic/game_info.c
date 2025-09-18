#include "game_info.h"

static Scene current_scene = SCENE_MAIN_MENU;

char string_buffer[64]; // buffer used for string formatting
f64 time_remaining; // game variable, counts down to game failure
int remaining_objectives; // track game progress, 0 = win

static void input_main_menu(Array* game_objects, f32 dt);
static void input_game(Array* game_objects, f32 dt);
static void input_game_over(Array* game_objects, f32 dt);
static void input_game_win(Array* game_objects, f32 dt);
static void update_main_menu(Array* game_objects, f32 dt);
static void update_game(Array* game_objects, f32 dt);
static void update_game_over(Array* game_objects, f32 dt);
static void update_game_win(Array* game_objects, f32 dt);
static void draw_main_menu(Array* game_objects);
static void draw_game(Array* game_objects);
static void draw_game_over(Array* game_objects);
static void draw_game_win(Array* game_objects);

static void scene_swap(Scene s, Array* game_objects);
static void game_init_objects(Array* game_objects);

void scene_input(Array* game_objects, f32 dt) {
	switch (current_scene) {
		case SCENE_MAIN_MENU:
			input_main_menu(game_objects, dt);
			break;
		case SCENE_GAME:
			input_game(game_objects, dt);
			break;
		case SCENE_GAME_OVER:
			input_game_over(game_objects, dt);
			break;
		case SCENE_WIN:
			input_game_win(game_objects, dt);
			break;
	}
}

void scene_update(Array* game_objects, f32 dt) {
	switch (current_scene) {
		case SCENE_MAIN_MENU:
			update_main_menu(game_objects, dt);
			break;
		case SCENE_GAME:
			update_game(game_objects, dt);
			break;
		case SCENE_GAME_OVER:
			update_game_over(game_objects, dt);
			break;
		case SCENE_WIN:
			update_game_win(game_objects, dt);
			break;
	}
}

void scene_draw(Array* game_objects) {
	switch (current_scene) {
	case SCENE_MAIN_MENU:
		draw_main_menu(game_objects);
		break;
	case SCENE_GAME:
		draw_game(game_objects);
		break;
	case SCENE_GAME_OVER:
		draw_game_over(game_objects);
		break;
	case SCENE_WIN:
		draw_game_win(game_objects);
		break;
	}
}

static void input_main_menu(Array* game_objects, f32 dt) {
	if (input_is_key_pressed(SDL_SCANCODE_SPACE)) {
		scene_swap(SCENE_GAME, game_objects);
	}
}

static void input_game(Array* game_objects, f32 dt) {
	for (int i = 0; i < game_objects->length; i++) {
		GameObject* g = &((GameObject*)game_objects->data)[i];
		if (g->input) g->input(g, dt);
	}
}

static void input_game_over(Array* game_objects, f32 dt) {
	if (input_is_key_pressed(SDL_SCANCODE_SPACE)) {
		scene_swap(SCENE_GAME, game_objects);
	}
}

static void input_game_win(Array* game_objects, f32 dt) {
	if (input_is_key_pressed(SDL_SCANCODE_SPACE)) {
		scene_swap(SCENE_GAME, game_objects);
	}
}

static void update_main_menu(Array* game_objects, f32 dt) {
	GameObject* g = &((GameObject*)game_objects->data)[0];
	mat4x4_rotate(g->transform.rotation, g->transform.rotation, 0.4f, 0.1f, 0.0f, dt * 0.3f);
}

static void update_game(Array* game_objects, f32 dt) {
	time_remaining -= (f64)dt;

	for (int i = 0; i < game_objects->length; i++) {
		GameObject* g = &((GameObject*)game_objects->data)[i];
		if (g->update) g->update(g, dt);
	}

	// removing objectives that have been collected.
	int l = game_objects->length;
	for (int i = 0; i < l; i++) {
		GameObject* g = &((GameObject*)game_objects->data)[i];
		if (g->obj_type == OBJECT_OBJECTIVE) {
			ObjectiveInfo* o = (ObjectiveInfo*)g->additional_data;
			if (o->is_collected) {
				array_remove_swapback(game_objects, i);
				i--;
				l--;
				remaining_objectives--;
			}
		}
	}

	if (remaining_objectives == 0) {
		scene_swap(SCENE_WIN, game_objects);
	}

	if (time_remaining < 0.0 || player_has_player_crashed()) {
		// lose conditions hit
		scene_swap(SCENE_GAME_OVER, game_objects);
	}
}

static void update_game_over(Array* game_objects, f32 dt) {
	GameObject* g = &((GameObject*)game_objects->data)[0];
	mat4x4_rotate(g->transform.rotation, g->transform.rotation, 0.4f, 0.1f, 0.0f, dt * 0.3f);
}

static void update_game_win(Array* game_objects, f32 dt) {
	GameObject* g = &((GameObject*)game_objects->data)[0];
	mat4x4_rotate(g->transform.rotation, g->transform.rotation, 0.4f, 0.1f, 0.0f, dt * 0.3f);
}

static void draw_main_menu(Array* game_objects) {
	Vector2f title_pos = {
		.x = 32,
		.y = 32
	};
	Rectangle src_temp = {
		.x = 0,
		.y = 0,
		.width = 70,
		.height = 10
	};
	Rectangle title_bg_dst = {
		.x = 30,
		.y = 21,
		.width = 304,
		.height = 48
	};
	Vector2f controls_pos = {
		.x = 32,
		.y = 96
	};

	spritebatch_begin();
	spritebatch_draw(title_bg_dst, src_temp, get_background_color_texture(), (Vector3f) { 1, 1, 1 });
	spritebatch_draw_string(
		title_pos,
		get_font(),
		"RING PLANE",
		3.0f,
		(Vector3f) { 0.2f, 0.9f, 0.16f }
	);
	spritebatch_draw_string(
		controls_pos,
		get_font(),
		"CONTROLS:",
		1.0f,
		(Vector3f) { 1, 1, 1 }
	);
	controls_pos.y += 16.0f;
	spritebatch_draw_string(
		controls_pos,
		get_font(),
		"THRUST - SHIFT / SPACE",
		1.0f,
		(Vector3f) { 1, 1, 1 }
	);
	controls_pos.y += 16.0f;
	spritebatch_draw_string(
		controls_pos,
		get_font(),
		"PITCH - W / S",
		1.0f,
		(Vector3f) { 1, 1, 1 }
	);
	controls_pos.y += 16.0f;
	spritebatch_draw_string(
		controls_pos,
		get_font(),
		"YAW - Q / E",
		1.0f,
		(Vector3f) { 1, 1, 1 }
	);
	controls_pos.y += 16.0f;
	spritebatch_draw_string(
		controls_pos,
		get_font(),
		"ROLL - A / D",
		1.0f,
		(Vector3f) { 1, 1, 1 }
	);
	controls_pos.y += 32.0f;
	spritebatch_draw_string(
		controls_pos,
		get_font(),
		"PRESS SPACE TO START",
		2.0f,
		(Vector3f) { 1, 1, 1}
	);
	spritebatch_end();
}

static void draw_game(Array* game_objects) {
	for (int i = 0; i < game_objects->length; i++) {
		GameObject* g = &((GameObject*)game_objects->data)[i];
		if (g->draw) g->draw(g);
		else renderer_draw_game_object(g);
	}

	Rectangle src_temp = {
		// here because theres no function
		// to draw with rect dst and no src
		.x = 0,
		.y = 0,
		.width = 70,
		.height = 10
	};
	Rectangle bg_dst = {
		.x = 0,
		.y = 0,
		.width = 188,
		.height = 12
	};
	Vector2f text_pos = {
		.x = 0,
		.y = 0
	};

	//thrust meter
	f32 thrust_mapped = (player_get_thrust() - 1.0f) / (3.0f - 1.0f) * (80.0f);
	Rectangle thrust_fg = {
		.x = 384,
		.y = 224,
		// map to 0->1 and scale by meter width
		.width = (int)thrust_mapped,
		.height = 32
	};

	// UI Stage
	// how do i access the fonts and shapes and such?
	spritebatch_begin();
	spritebatch_draw_vec_dst((Vector2f) { 0, 0 }, get_game_overlay(), (Vector3f) { 1, 1, 1 });
	spritebatch_draw(thrust_fg, src_temp, get_background_color_texture(), (Vector3f) { 0, 1, 0 });
	spritebatch_draw(bg_dst, src_temp, get_background_color_texture(), (Vector3f) { 0, 0, 1 });
	snprintf(string_buffer, sizeof(string_buffer), "TIME LEFT - %.2f", time_remaining);
	spritebatch_draw_string(
		text_pos,
		get_font(),
		string_buffer,
		1.0f,
		(Vector3f) { 1, 1, 1 }
	);
	spritebatch_end();
}

static void draw_game_over(Array* game_objects) {
	// text : game over ... press space to restart
	Vector2f text_pos = {
		.x = 32,
		.y = 48
	};

	spritebatch_begin();
	spritebatch_draw_string(
		text_pos,
		get_font(),
		"YOU LOSE :(",
		2.0f,
		(Vector3f) { 0.8f, 0.1f, 0.24f }
	);
	text_pos.y += 32;
	spritebatch_draw_string(
		text_pos,
		get_font(),
		"PRESS SPACE TO RETRY",
		1.0f,
		(Vector3f) { 1, 1, 1 }
	);
	spritebatch_end();
}

static void draw_game_win(Array* game_objects) {
	Vector2f smiley_dst = {
		.x = 32,
		.y = 32
	};
	Vector2f text_pos = {
		.x = 104,
		.y = 32
	};

	spritebatch_begin();
	spritebatch_draw_vec_dst(smiley_dst, get_game_winner_sprite(), (Vector3f){ 1, 1, 1 });
	spritebatch_draw_string(
		text_pos,
		get_font(),
		"YOU WIN!",
		3.0f,
		(Vector3f) { 0.2f, 0.9f, 0.16f }
	);
	text_pos.x -= 72;
	text_pos.y += 72;
	spritebatch_draw_string(
		text_pos,
		get_font(),
		"PRESS SPACE TO PLAY AGAIN",
		1.0f,
		(Vector3f) { 1, 1, 1 }
	);
	spritebatch_end();
}

static void game_init_objects(Array* game_objects) {
	// free dummy objects
	array_free(game_objects);
	*game_objects = array_init(sizeof(GameObject));

	rng_set_seed(SDL_GetTicks());

	time_remaining = 120.0;
	remaining_objectives = 10;

	fnl_state noise = fnlCreateState();
	noise.seed = (int)rng_next_double(0, 30000);
	noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
	noise.frequency = 0.1f;

	GameObject terrain;
	game_object_init(&terrain, RENDER_TERRAIN, OBJECT_MAP);
	mesh_load_from_heightmap(&terrain.mesh, &noise, 256, 256);
	texture_load_texture(&terrain.texture, "./res/textures/terrain_dither.png", 0);
	terrain.transform.scale = 1.0f;
	array_append(game_objects, &terrain);

	// player
	GameObject ship;
	game_object_init(&ship, RENDER_DEFAULT, OBJECT_PLAYER);
	mesh_load_from_obj(&ship.mesh, "./res/models/ship.obj");
	texture_load_from_color(&ship.texture, (u8[4]) { 80, 10, 25, 255 });
	ship.transform.position.y += 5.0f;
	ship.transform.scale = 0.3f;
	player_info_init(&ship, &noise);
	ship.input = player_input;
	ship.update = player_update;
	array_append(game_objects, &ship);

	// objectives
	for (int i = 0; i < remaining_objectives; i++) {
		GameObject ring;
		game_object_init(&ring, RENDER_DEFAULT, OBJECT_OBJECTIVE);
		mesh_load_from_obj(&ring.mesh, "./res/models/ring.obj");
		texture_load_from_color(&ring.texture, (u8[4]) { 15, 200, 45, 125 });
		mat4x4_rotate(ring.transform.rotation, ring.transform.rotation, 1, 0, 0, degree_to_rad(90.0f));
		mat4x4_rotate(
			ring.transform.rotation,
			ring.transform.rotation,
			0, 0, 1,
			degree_to_rad(rng_next_double(0.0, 360.0))
		);
		f64 x_pos = rng_next_double(-128.0, 128.0);
		f64 z_pos = rng_next_double(-128.0, 128.0);
		ring.transform.position.x = x_pos;
		ring.transform.position.z = z_pos;
		ring.transform.scale = 2.0f;
		objective_init(&ring);
		ring.update = objective_update;
		array_append(game_objects, &ring);
	}

	camera_change_target(&(((GameObject*)game_objects->data)[1].transform));
}

static void scene_swap(Scene s, Array* game_objects) {
	if (current_scene == SCENE_GAME) {
		for (int i = 0; i < game_objects->length; i++) {
			GameObject* g = game_objects->data;
			game_object_clean(&g[i]);
		}
		// make a game object instead of a transform
		// so we can put it in the array and it doesnt go out of memory
		GameObject g;
		game_object_init(&g, 0, 0);
		array_append(game_objects, &g);
		camera_change_target(&(((GameObject*)game_objects->data)[0].transform));
	}
	if (s == SCENE_GAME) {
		game_init_objects(game_objects);
	}
	current_scene = s;
}

void game_init(Array* game_objects) {
	GameObject g;
	game_object_init(&g, 0, 0);
	array_append(game_objects, &g);
	camera_chase_init(&(((GameObject*)game_objects->data)[0].transform), (vec3) { 0.f, 1.f, 4.5f });
}

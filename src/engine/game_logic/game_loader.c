#include "game_loader.h"

static Font game_font = { 0 };
static Texture game_color_texture = { 0 };
static Texture game_overlay_texture = { 0 };
static Texture game_winner_texture = { 0 };

Texture* get_background_color_texture() { return &game_color_texture; }
Font* get_font() { return &game_font; }
Texture* get_game_overlay() { return &game_overlay_texture; }
Texture* get_game_winner_sprite() { return &game_winner_texture; }

void game_global_init() {
	// I shouldn't have to worry about cleaning this as copying the
	// struct to font means that font will not store the same tex ID,
	// so cleaning there should handle cleaning here
	Texture font_tex;
	texture_init(&font_tex);
	texture_load_texture(&font_tex, "./res/textures/font-testing.png");
	game_font = font_init("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!?.,/-%()[]\":#", font_tex, 10, 10);
	texture_init(&game_color_texture);
	texture_load_from_color(&game_color_texture, (u8[4]) { 255, 255, 255, 255 });
	texture_init(&game_overlay_texture);
	texture_load_texture(&game_overlay_texture, "./res/textures/overlay.png");
	texture_init(&game_winner_texture);
	texture_load_texture(&game_winner_texture, "./res/textures/smiley.png");
}

void game_assets_unload() {
	font_clean(&game_font);
	texture_clean(&game_color_texture);
	texture_clean(&game_overlay_texture);
	texture_clean(&game_winner_texture);
}

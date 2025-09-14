#ifndef GAME_LOADER_H
#define GAME_LAODER_H

// this is going to load a bunch of stuff that I want to use across scenes, or just
// stuff that cant go in the game object array
// will have a single init function, then loaders to grab static stuff

#include "../math/rectangle.h"
#include "../math/vector.h"
#include "../rendering/texture.h"
#include "../rendering/font.h"

void game_global_init();

Texture* get_background_color_texture();
Texture* get_game_overlay();
Font* get_font();
Texture* get_game_winner_sprite();
void game_assets_unload();

#endif
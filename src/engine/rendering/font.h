#ifndef FONT_H
#define FONT_H

#include "texture.h"
#include "../math/rectangle.h"

#include "string.h"

typedef struct font {
	int char_index[128];
	Rectangle* char_sources;
	Texture fontset;
	int char_width, char_height;
} Font;

Font font_init(const char* charset, Texture f, int char_width, int char_height);
Rectangle font_get_src(char c, Font* f);
void font_clean(Font* f);

#endif
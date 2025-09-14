#include "font.h"

//TODO! since I store all the rectangles already, it would be nice
//	to have support for not using monospace, only issue is that would
//	pretty much require me to read in some kind of metadata file
//	to calculate the sources which is a bit effort

Font font_init(const char* charset, Texture f, int char_width, int char_height) {
	Font font = {0};

	font.char_sources = malloc(sizeof(Rectangle) * strlen(charset));
	font.fontset = f;
	font.char_width = char_width;
	font.char_height = char_height;

	int columns = f.width / char_width;

	memset(font.char_index, -1, sizeof(font.char_index));

	for (int i = 0; charset[i]; i++) {
		font.char_index[(int)charset[i]] = i;
		// calculate source
		int column = i % columns;
		int row = i / columns;

		Rectangle rect = {
			.x = column * char_width,
			.y = row * char_height,
			.width = char_width,
			.height = char_height
		};

		font.char_sources[i] = rect;
	}

	return font;
}

Rectangle font_get_src(char c, Font* f) {
	int index = f->char_index[c];

	if (index == -1) {
		Rectangle empty = {.x=0,.y=0,.width=0,.height=0};
		return empty;
	}

	return f->char_sources[index];
}

void font_clean(Font* f) {
	free(f->char_index);
	free(f->char_sources);
	texture_clean(&f->fontset);

	f->char_sources = NULL;
}

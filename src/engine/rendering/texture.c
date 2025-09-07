#include "texture.h"

void texture_init(Texture* t) {
	glGenTextures(1, &t->id);
	t->width = 0;
	t->height = 0;
}

void texture_load_texture(Texture* t, const char* texturePath) {
	texture_bind(&t->id);

	//TODO a flip flag would be nice
	// some textures will be loaded with the wrong orientation
	// and I should have a funtion arg to set stbi_flip_vertically
	// instead of needing to configure the files manually

	// set sampler parameters, really should be configurable
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, channels;
	unsigned char* data = stbi_load(texturePath, &width, &height, &channels, 0);

	if (!data) {
		// Texture couldn't be read, fallback to this
		fprintf(stderr, "Error reading texture: %s - defaulting\n", texturePath);
		u8 color[4] = {0, 255, 0, 255 };
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, color);
		texture_unbind();
		return;
	}

	GLenum format;
	switch (channels) {
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			ERROR_EXIT("Attempted to load a texture that was not 3 or 4 channels. Look into this: %s", texturePath);
			break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	// not generating mipmaps for now, but might want to later
	// if I do, I'll need to change the MIN_FILTER to MIPMAP_LINEAR or MIPMAP_NEAREST
	glGenerateMipmap(GL_TEXTURE_2D);

	t->width = width;
	t->height = height;

	texture_unbind();

	stbi_image_free(data);
}

void texture_load_cube_texture(Texture* t, const char** texturePaths) {
	texture_bind(&t->id);

	// set sampler parameters, really should be configurable
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	int width, height, channels;
	unsigned char* data;
	//unsigned char* data = stbi_load(texturePath, &width, &height, &channels, 0);
	// since we use a pointer, hard to get length, dont wanna pass in number right
	// now so just hardcoding it
	for (int i = 0; i < 6; i++) {
		data = stbi_load(texturePaths[i], &width, &height, &channels, 0);
		if (!data) {
			ERROR_EXIT("Error reading cubemap face: %s\n", texturePaths[i]);
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 
					GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
		t->height += height;
		t->width += width;
	}

	texture_unbind();
}

void texture_load_from_color(Texture* t, u8 color[4]) {
	texture_bind(&t->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, color);
	texture_unbind();

	t->width = 1;
	t->height = 1;
	
	return;
}

void texture_bind(tex_id* t) {
	//TODO! I previously had this below line hardcoded into this function
	//	but if I ever want a way to have multiple textures binded
	//  e.g. normal maps, I'd need a way to configure the texture slot
	//	im making active. I can pass an int and just add it to the 0 arg
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *t);
}

void texture_unbind(void) {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_clean(Texture* t) {
	t->width = 0;
	t->height = 0;
	texture_delete(&t->id);
}

void texture_delete(tex_id* t) {
	glDeleteTextures(1, t);
}

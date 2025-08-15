#include "texture.h"

void texture_init(u32* id) {
	glGenTextures(1, id);
}

void texture_load_texture(u32* id, const char* texturePath) {
	texture_bind(id);

	// set sampler parameters, really should be configurable
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, channels;
	unsigned char* data = stbi_load(texturePath, &width, &height, &channels, 0);

	if (!data) {
		ERROR_EXIT("Error reading texture: %s\n", texturePath);
	}

	// TODO
	// this currently loads in 4 channels no matter what - if I load a jpeg it would probably
	// break. Might be worth actually using the channels value to grab the correct format
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	
	// not generating mipmaps for now, but might want to later
	// if I do, I'll need to change the MIN_FILTER to MIPMAP_LINEAR or MIPMAP_NEAREST
	//glGenerateMipmap(GL_TEXTURE_2D);

	texture_unbind();

	stbi_image_free(data);
}

void texture_bind(u32* id) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *id);
}

void texture_unbind(void) {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_clean(u32* id) {
	glDeleteTextures(1, id);
}

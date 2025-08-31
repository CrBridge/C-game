#include "fbo.h"

void fbo_init(fbo* id) {
	glGenFramebuffers(1, id);
}

void fbo_add_buffer(tex_id* id, u16 width, u16 height) {
	//TODO! bit hardcoded, could probably rework some of the
	//	texture functions so I can call those instead
	glGenTextures(1, id);
	glBindTexture(GL_TEXTURE_2D, *id);

	//colour attachment, pass in RGBA4 for internalformat to reduce the color depth to 16bit
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *id, 0);
}

void fbo_bind(fbo* id) {
	glBindFramebuffer(GL_FRAMEBUFFER, *id);
}

void fbo_unbind(void) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void fbo_clean(fbo* id) {
	glDeleteFramebuffers(1, id);
}

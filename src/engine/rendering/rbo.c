#include "rbo.h"

void rbo_init(rbo* id) {
	glGenRenderbuffers(1, id);
}

void rbo_attach_depth_stencil_buffers(rbo* id, u16 width, u16 height) {
	rbo_bind(id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	rbo_unbind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *id);
}

void rbo_bind(rbo* id) {
	glBindRenderbuffer(GL_RENDERBUFFER, *id);
}

void rbo_unbind(void) {
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void rbo_clean(rbo* id) {
	glDeleteRenderbuffers(1, id);
}

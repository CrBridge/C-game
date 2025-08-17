#include "vao.h"

void vao_init(vao* id) {
	glGenVertexArrays(1, id);
}

void vao_setup_vbo_attrib(vao* vboId, u32 layout, u32 size, GLenum type, size_t stride, void* offset) {
	vbo_bind(vboId);

	glVertexAttribPointer(layout, size, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);

	vbo_unbind();
}

void vao_bind(vao* id) {
	glBindVertexArray(*id);
}

void vao_unbind(void) {
	glBindVertexArray(0);
}

void vao_clean(vao* id) {
	glDeleteVertexArrays(1, id);
}

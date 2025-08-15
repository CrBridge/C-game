#include "vao.h"

void vao_init(u32* id) {
	glGenVertexArrays(1, id);
}

void vao_setup_vbo_attrib(u32* vboId, u32 layout, u32 size, GLenum type, size_t stride, void* offset) {
	vbo_bind(vboId);

	glVertexAttribPointer(layout, size, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);

	vbo_unbind();
}

void vao_bind(u32* id) {
	glBindVertexArray(*id);
}

void vao_unbind(void) {
	glBindVertexArray(0);
}

void vao_clean(u32* id) {
	glDeleteVertexArrays(1, id);
}

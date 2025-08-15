#include "vbo.h"

void vbo_init(u32* id) {
	glGenBuffers(1, id);
}

void vbo_add_data(u32* id, Vertex* vertices, u32 vertexCount) {
	vbo_bind(id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexCount, vertices, GL_STATIC_DRAW);
}

void vbo_bind(u32* id) {
	glBindBuffer(GL_ARRAY_BUFFER, *id);
}

void vbo_unbind(void) {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vbo_clean(u32* id) {
	glDeleteBuffers(1, id);
}

#include "ebo.h"

void ebo_init(ebo* id) {
	glGenBuffers(1, id);
}

void ebo_add_data(ebo* id, u32* indices, u32 indexCount) {
	ebo_bind(id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * indexCount, indices, GL_STATIC_DRAW);
}

void ebo_bind(ebo* id) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *id);
}

void ebo_unbind(void) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ebo_clean(ebo* id) {
	glDeleteBuffers(1, id);
}

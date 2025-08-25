#include "sprite_batch.h"

typedef struct spritebatch {
	Array indices;
	Array vertices;
	shader* bound_shader;
	Texture* bound_texture;
	vbo vbo;
	vao vao;
	ebo ebo;
} SpriteBatch;

static SpriteBatch batch = { 0 };
static mat4x4 projection;

void spritebatch_init(int nativeWidth, int nativeHeight) {
	batch.indices = array_init(sizeof(u32));
	batch.vertices = array_init(sizeof(Vertex));

	batch.bound_shader = NULL;
	batch.bound_texture = NULL;

	vbo_init(&batch.vbo);
	vao_init(&batch.vao);
	ebo_init(&batch.ebo);

	vao_bind(&batch.vao);

	// TODO! normals are a bit useless here
	//	again, a way to not use them would be nice
	vao_setup_vbo_attrib(&batch.vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
	vao_setup_vbo_attrib(&batch.vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	vao_setup_vbo_attrib(&batch.vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	mat4x4_ortho(projection, 0, nativeWidth, nativeHeight, 0, 0.1f, 100.0f);
}

void spritebatch_begin(shader* s) {
	batch.bound_shader = s;
	//memcpy(batch.matrix, m, sizeof(mat4x4));
}

void spritebatch_draw(Rectangle dst, Rectangle src, Texture* tex) {
	if (batch.bound_texture && batch.bound_texture->id != tex->id) {
		spritebatch_end();
		spritebatch_begin(batch.bound_shader);
	}
	batch.bound_texture = tex;

	u32 index = batch.vertices.length;

	f32 l = (f32)src.x / (f32)tex->width;
	f32 t = (f32)src.y / (f32)tex->height;
	f32 r = (f32)(src.x + src.width) / (f32)tex->width;
	f32 b = (f32)(src.y + src.height) / (f32)tex->height;

	// Create and append vertices: TR-BR-BL-TL
	array_append(&batch.vertices, &(Vertex) { .position = (Vector3){ dst.x + dst.width, dst.y, -1.0f }, .normal = (Vector3){ 0, 0, 0 }, .uv = (Vector2){ r, t } });
	array_append(&batch.vertices, &(Vertex) { .position = (Vector3){ dst.x + dst.width, dst.y + dst.height, -1.0f }, .normal = (Vector3){ 0, 0, 0 }, .uv = (Vector2){ r, b } });
	array_append(&batch.vertices, &(Vertex) { .position = (Vector3){ dst.x, dst.y + dst.height, -1.0f }, .normal = (Vector3){ 0, 0, 0 }, .uv = (Vector2){ l, b } });
	array_append(&batch.vertices, &(Vertex) { .position = (Vector3){ dst.x, dst.y, -1.0f }, .normal = (Vector3){ 0, 0, 0 }, .uv = (Vector2){ l, t } });

	u32 i0 = index + 0;
	u32 i1 = index + 1;
	u32 i2 = index + 2;
	u32 i3 = index + 3;
	array_append(&batch.indices, &i3);
	array_append(&batch.indices, &i2);
	array_append(&batch.indices, &i1);
	array_append(&batch.indices, &i3);
	array_append(&batch.indices, &i1);
	array_append(&batch.indices, &i0);
}

void spritebatch_render() {
	if (!batch.bound_shader || !batch.bound_texture) return;

	vao_bind(&batch.vao);
	vbo_bind(&batch.vbo);
	ebo_bind(&batch.ebo);

	vbo_add_data(&batch.vbo, (Vertex*)batch.vertices.data, batch.vertices.length);
	ebo_add_data(&batch.ebo, (u32*)batch.indices.data, batch.indices.length);

	shader_use(batch.bound_shader);
	texture_bind(&batch.bound_texture->id);

	shader_set_mat4(batch.bound_shader, "projection", &projection);

	glDrawElements(GL_TRIANGLES, (GLsizei)batch.indices.length, GL_UNSIGNED_INT, NULL);
}

void spritebatch_end() {
	spritebatch_render();

	// no clear function rigth now so we just free and re-init
	array_free(&batch.vertices);
	array_free(&batch.indices);

	batch.indices = array_init(sizeof(u32));
	batch.vertices = array_init(sizeof(Vertex));
}

void spritebatch_clean() {
	array_free(&batch.vertices);
	array_free(&batch.indices);

	batch.bound_shader = NULL;
	batch.bound_texture = NULL;

	vbo_clean(&batch.vbo);
	vao_clean(&batch.vao);
	ebo_clean(&batch.ebo);
}

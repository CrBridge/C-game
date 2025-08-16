#ifndef MESH_H
#define MESH_H

#include "../types.h"
#include "../io/io.h"
#include "../util.h"
#include "../io/io.h"

#include "vertex.h"
#include "vbo.h"
#include "vao.h"
#include "ebo.h"

#include <linmath.h>
#include <glad/glad.h>
#include <stdio.h>

typedef struct mesh {
	u32 vbo;
	u32 vao;
	u32 ebo;
	u32 indexCount;
} Mesh;

void mesh_load_from_obj(Mesh* mesh, const char* filePath);
void mesh_load_from_memory(Mesh* mesh, Vertex* vertices, u32 vertexCount, u32* indices, u32 indexCount);
void mesh_draw(Mesh* mesh);
void mesh_clean(Mesh* mesh);

#endif

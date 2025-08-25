#ifndef MESH_H
#define MESH_H

#include "../types.h"
#include "../io/io.h"
#include "../util.h"
#include "../data/array.h"
#include "../data/map.h"

#include "vertex.h"
#include "vbo.h"
#include "vao.h"
#include "ebo.h"

#include <linmath.h>
#include <glad/glad.h>
#include <stdio.h>

//
#include <FastNoiseLite.h>

typedef struct mesh {
	vbo vbo;
	vao vao;
	ebo ebo;
	u32 indexCount;
} Mesh;

void mesh_load_from_obj(Mesh* mesh, const char* filePath);
void mesh_load_from_memory(Mesh* mesh, Vertex* vertices, u32 vertexCount, u32* indices, u32 indexCount);
void mesh_draw(Mesh* mesh);
void mesh_clean(Mesh* mesh);

// some helpers for generating meshes without the need of a file or manually specifying vertices
// may have an enum for these shapes so that any larger struct init function (e.g. an init_game_object)
// can easily access them using it
void mesh_load_cube(Mesh* mesh);
void mesh_load_quad(Mesh* mesh);
void mesh_load_screen_quad(Mesh* mesh);
void mesh_load_sky_cube(Mesh* mesh);

#endif

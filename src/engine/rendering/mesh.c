#include "mesh.h"

void mesh_load_from_obj(Mesh* mesh, const char* filePath) {
	//TODO! still not great, but the bottleneck at this point is
	//	the vertex deduplication, and just the fact that I am reading text with fscanf
	//	the fscanf could probably be sped up, but If I spend more time on this it should
	//	be on improving the deduplication loop

	Array positions = array_init(sizeof(vec3));
	Array normals = array_init(sizeof(vec3));
	Array uvs = array_init(sizeof(vec2));

	Array positionIndices = array_init(sizeof(u32));
	Array normalIndices = array_init(sizeof(u32));
	Array uvIndices = array_init(sizeof(u32));

	FILE* fp = fopen(filePath, "r");

	if (!fp) {
		ERROR_EXIT(IO_READ_ERROR_GENERAL, filePath, errno);
	}

	while (1) {
		char lineHeader[128] = {0};
		int res = fscanf(fp, "%s", lineHeader);
		if (res == EOF) {
			break;
		}

		if (strcmp(lineHeader, "v") == 0) {
			vec3 position;
			(void)fscanf(fp, "%f %f %f\n", &position[0], &position[1], &position[2]);
			array_append(&positions, &position);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			vec2 uv;
			(void)fscanf(fp, "%f %f\n", &uv[0], &uv[1]);
			array_append(&uvs, &uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			vec3 normal;
			(void)fscanf(fp, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
			array_append(&normals, &normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			u32 positionIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(
				fp,
				"%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&positionIndex[0], &uvIndex[0], &normalIndex[0],
				&positionIndex[1], &uvIndex[1], &normalIndex[1],
				&positionIndex[2], &uvIndex[2], &normalIndex[2]
			);
			if (matches != 9) {
				ERROR_EXIT("Error reading face information for mesh initialisation: %s\n", filePath);
			}
			for (int k = 0; k < 3; k++) {
				array_append(&positionIndices, &positionIndex[k]);
				array_append(&normalIndices, &normalIndex[k]);
				array_append(&uvIndices, &uvIndex[k]);
			}
		}
	}
	fclose(fp);

	Array meshVertices = array_init(sizeof(Vertex));
	Array meshIndices = array_init(sizeof(u32));

	u32* finalPositionIndices = (u32*)positionIndices.data;
	u32* finalNormalIndices = (u32*)normalIndices.data;
	u32* finalUvIndices = (u32*)uvIndices.data;

	for (u32 i = 0; i < positionIndices.length; i++) {
		Vertex v = { 0 };

		u32 positionIndex = finalPositionIndices[i] - 1;
		u32 normalIndex = finalNormalIndices[i] - 1;
		u32 uvIndex = finalUvIndices[i] - 1;

		memcpy(v.position, array_get(&positions, positionIndex), sizeof(vec3));
		memcpy(v.normal, array_get(&normals, normalIndex), sizeof(vec3));
		memcpy(v.uv, array_get(&uvs, uvIndex), sizeof(vec2));

		i32 existingIndex = -1;
		Vertex* existingVertices = (Vertex*)meshVertices.data;
		// TODO! really bad performance, looping the entire thing just to check if a vertex is
		// equal, Hash functionality would be much nicer :)
		for (u32 j = 0; j < meshVertices.length; j++) {
			if (vertices_are_equal(&existingVertices[j], &v)) {
				existingIndex = j;
				break;
			}
		}

		if (existingIndex >= 0) {
			array_append(&meshIndices, &existingIndex);
		}
		else {
			array_append(&meshVertices, &v);
			u32 newIndex = (u32)(meshVertices.length - 1);
			array_append(&meshIndices, &newIndex);
		}
	}

	mesh_load_from_memory(
		mesh, 
		(Vertex*)meshVertices.data, 
		(u32)meshVertices.length, 
		(u32*)meshIndices.data, 
		(u32)meshIndices.length
	);

	array_free(&positions);
	array_free(&positionIndices);
	array_free(&uvs);
	array_free(&uvIndices);
	array_free(&normals);
	array_free(&normalIndices);

	array_free(&meshVertices);
	array_free(&meshIndices);
}

// TODO! WIP
void mesh_load_from_heightmap(Mesh* mesh) {
	Vertex* meshVertices = NULL;
	u32 vertexCount = 0;
	u32* meshIndices = NULL;
	u32 indexCount = 0;

	fnl_state noise = fnlCreateState();
	noise.noise_type = FNL_NOISE_OPENSIMPLEX2;

	for (int x = 0; x < 128; x++) {
		for (int y = 0; y < 128; y++) {
			// get noise value for a vertex
			float heightVal = fnlGetNoise2D(&noise, x, y);
		}
	}

	mesh_load_from_memory(mesh, meshVertices, vertexCount, meshIndices, indexCount);
}

void mesh_load_from_memory(Mesh* mesh, Vertex* vertices, u32 vertexCount, u32* indices, u32 indexCount) {
	vbo_init(&mesh->vbo);
	vao_init(&mesh->vao);
	ebo_init(&mesh->ebo);

	vao_bind(&mesh->vao);

	vbo_bind(&mesh->vbo);
	vbo_add_data(&mesh->vbo, vertices, vertexCount);

	ebo_bind(&mesh->ebo);
	ebo_add_data(&mesh->ebo, indices, indexCount);

	// static vertex declaration, it being configurable would be nice
	vao_setup_vbo_attrib(&mesh->vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
	vao_setup_vbo_attrib(&mesh->vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	vao_setup_vbo_attrib(&mesh->vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	vao_unbind();

	mesh->indexCount = indexCount;
}

void mesh_draw(Mesh* mesh) {
	vao_bind(&mesh->vao);
	// I pass null here, when I have multiple models, will I need to ensure the ebo is binded again,
	// or is that all just on init, and then I don't have to worry about it again?
	glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, NULL);
}

void mesh_clean(Mesh* mesh) {
	ebo_clean(&mesh->ebo);
	vao_clean(&mesh->vao);
	vbo_clean(&mesh->vbo);
}

// ===================== Mesh Helpers ==================== //
void mesh_load_cube(Mesh* mesh) {
	Vertex vertices[] = {
		{{-0.5, -0.5,  0.5}, {0, 0, 1}, {0, 0}},
		{{ 0.5, -0.5,  0.5}, {0, 0, 1}, {1, 0}},
		{{ 0.5,  0.5,  0.5}, {0, 0, 1}, {1, 1}},
		{{-0.5,  0.5,  0.5}, {0, 0, 1}, {0, 1}},

		{{ 0.5, -0.5, -0.5}, {0, 0, -1}, {0, 0}},
		{{-0.5, -0.5, -0.5}, {0, 0, -1}, {1, 0}},
		{{-0.5,  0.5, -0.5}, {0, 0, -1}, {1, 1}},
		{{ 0.5,  0.5, -0.5}, {0, 0, -1}, {0, 1}},

		{{-0.5, -0.5, -0.5}, {-1, 0, 0}, {0, 0}},
		{{-0.5, -0.5,  0.5}, {-1, 0, 0}, {1, 0}},
		{{-0.5,  0.5,  0.5}, {-1, 0, 0}, {1, 1}},
		{{-0.5,  0.5, -0.5}, {-1, 0, 0}, {0, 1}},

		{{ 0.5, -0.5,  0.5}, {1, 0, 0}, {0, 0}},
		{{ 0.5, -0.5, -0.5}, {1, 0, 0}, {1, 0}},
		{{ 0.5,  0.5, -0.5}, {1, 0, 0}, {1, 1}},
		{{ 0.5,  0.5,  0.5}, {1, 0, 0}, {0, 1}},

		{{-0.5,  0.5,  0.5}, {0, 1, 0}, {0, 0}},
		{{ 0.5,  0.5,  0.5}, {0, 1, 0}, {1, 0}},
		{{ 0.5,  0.5, -0.5}, {0, 1, 0}, {1, 1}},
		{{-0.5,  0.5, -0.5}, {0, 1, 0}, {0, 1}},

		{{-0.5, -0.5, -0.5}, {0, -1, 0}, {0, 0}},
		{{ 0.5, -0.5, -0.5}, {0, -1, 0}, {1, 0}},
		{{ 0.5, -0.5,  0.5}, {0, -1, 0}, {1, 1}},
		{{-0.5, -0.5,  0.5}, {0, -1, 0}, {0, 1}},
	};

	u32 indices[] = {
		 0,  1,  2,  2,  3,  0,
		 4,  5,  6,  6,  7,  4,
		 8,  9, 10, 10, 11,  8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20
	};

	mesh_load_from_memory(mesh, vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
}

void mesh_load_quad(Mesh* mesh) {
	Vertex vertices[] = {
		{{-0.5, -0.5,  0.0}, {0, 0, 1}, {0, 0}},
		{{ 0.5, -0.5,  0.0}, {0, 0, 1}, {1, 0}},
		{{ 0.5,  0.5,  0.0}, {0, 0, 1}, {1, 1}},
		{{-0.5,  0.5,  0.0}, {0, 0, 1}, {0, 1}}
	};

	u32 indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	mesh_load_from_memory(mesh, vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
}

void mesh_load_screen_quad(Mesh* mesh) {
	//TODO! it's such a small mesh that the impact is almost certainly negligible,
	//	but having normal and the positions z component is completely useless and would
	//	be better off just have 2 vec2 and using a custom vertex declaration
	Vertex vertices[] = {
		{{-1.0, -1.0,  0.0}, {0, 0, 0}, {0, 0}},
		{{ 1.0, -1.0,  0.0}, {0, 0, 0}, {1, 0}},
		{{ 1.0,  1.0,  0.0}, {0, 0, 0}, {1, 1}},
		{{-1.0,  1.0,  0.0}, {0, 0, 0}, {0, 1}}
	};

	u32 indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	mesh_load_from_memory(mesh, vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
}

void mesh_load_sky_cube(Mesh* mesh) {
	//TODO! similar situation as screen_quad, except this one is alot more
	//	wasteful, in fact I only need position values here since
	//	the uv is calculated with the view matrix, should really rework this one
	Vertex vertices[] = {
		{{-1.0, -1.0,  1.0}, {0, 0, 1}, {0, 0}},
		{{ 1.0, -1.0,  1.0}, {0, 0, 1}, {1, 0}},
		{{ 1.0,  1.0,  1.0}, {0, 0, 1}, {1, 1}},
		{{-1.0,  1.0,  1.0}, {0, 0, 1}, {0, 1}},

		{{ 1.0, -1.0, -1.0}, {0, 0, -1}, {0, 0}},
		{{-1.0, -1.0, -1.0}, {0, 0, -1}, {1, 0}},
		{{-1.0,  1.0, -1.0}, {0, 0, -1}, {1, 1}},
		{{ 1.0,  1.0, -1.0}, {0, 0, -1}, {0, 1}},

		{{-1.0, -1.0, -1.0}, {-1, 0, 0}, {0, 0}},
		{{-1.0, -1.0,  1.0}, {-1, 0, 0}, {1, 0}},
		{{-1.0,  1.0,  1.0}, {-1, 0, 0}, {1, 1}},
		{{-1.0,  1.0, -1.0}, {-1, 0, 0}, {0, 1}},

		{{ 1.0, -1.0,  1.0}, {1, 0, 0}, {0, 0}},
		{{ 1.0, -1.0, -1.0}, {1, 0, 0}, {1, 0}},
		{{ 1.0,  1.0, -1.0}, {1, 0, 0}, {1, 1}},
		{{ 1.0,  1.0,  1.0}, {1, 0, 0}, {0, 1}},

		{{-1.0,  1.0,  1.0}, {0, 1, 0}, {0, 0}},
		{{ 1.0,  1.0,  1.0}, {0, 1, 0}, {1, 0}},
		{{ 1.0,  1.0, -1.0}, {0, 1, 0}, {1, 1}},
		{{-1.0,  1.0, -1.0}, {0, 1, 0}, {0, 1}},

		{{-1.0, -1.0, -1.0}, {0, -1, 0}, {0, 0}},
		{{ 1.0, -1.0, -1.0}, {0, -1, 0}, {1, 0}},
		{{ 1.0, -1.0,  1.0}, {0, -1, 0}, {1, 1}},
		{{-1.0, -1.0,  1.0}, {0, -1, 0}, {0, 1}},
	};

	// reversed so faces are on the inside
	u32 indices[] = {
		 20, 23, 22, 22, 21, 20, 
		 16, 19, 18, 18, 17, 16, 
		 12, 15, 14, 14, 13, 12,  
		  8, 11, 10, 10,  9,  8,  
		  4,  7,  6,  6,  5,  4,  
		  0,  3,  2,  2,  1,  0
	};

	mesh_load_from_memory(mesh, vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
}
// ======================================================= //

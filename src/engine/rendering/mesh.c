#include "mesh.h"

void mesh_load_from_obj(Mesh* mesh, const char* filePath) {
	//TODO! pretty good at this point
	//	only thing to optimize at this point is the scanf usage
	//	and I'm probably using more arrays than actually needed
	//	Actually, I suppose if I declare unique_vertices at the start,
	//	I could try constructing the vertex structs in the same pass I'm
	//	reading the files and cut out the 2nd pass entirely

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

	Map* uniqueVertices = map_create(vertices_map_hash, vertices_map_eq);

	for (u32 i = 0; i < positionIndices.length; i++) {
		Vertex v = { 0 };

		u32 positionIndex = finalPositionIndices[i] - 1;
		u32 normalIndex = finalNormalIndices[i] - 1;
		u32 uvIndex = finalUvIndices[i] - 1;

		// TODO! cant I just assign this directly?
		memcpy(&v.position, array_get(&positions, positionIndex), sizeof(vec3));
		memcpy(&v.normal, array_get(&normals, normalIndex), sizeof(vec3));
		memcpy(&v.uv, array_get(&uvs, uvIndex), sizeof(vec2));

		u32* existingIndex = (u32*)map_get(uniqueVertices, &v, sizeof(Vertex));

		if (existingIndex) {
			array_append(&meshIndices, existingIndex);
		}
		else {
			u32 newIndex = (u32)meshVertices.length;
			array_append(&meshVertices, &v);
			map_set(uniqueVertices, &v, sizeof(Vertex), &newIndex, sizeof(u32));
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

	map_free(uniqueVertices);
}

static Vector3f mesh_calculate_terrain_normal(const fnl_state* state, int x, int y) {
	float dx = fnlGetNoise2D(state, x + 1, y) - fnlGetNoise2D(state, x - 1, y);
	float dy = fnlGetNoise2D(state, x, y + 1) - fnlGetNoise2D(state, x, y - 1);

	vec3 n  = { -dx, 1, -dy };
	vec3_norm(n, n);

	//printf("normal value for y - %f\n", n[1]);

	return (Vector3f) { n[0], n[1], n[2] };
}

void mesh_load_from_heightmap(Mesh* mesh, const fnl_state* state, u16 width, u16 height) {
	Array vertices = array_init(sizeof(Vertex));
	Array indices = array_init(sizeof(u32));

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			float height_val = fnlGetNoise2D(state, x, y);
			Vertex vertex = { 0 };
			vertex.position = (Vector3f) {
				-width / 2.0f + width * x / (float)width,
				height_val,
				-height / 2.0f + height * y / (float)height
				};
			vertex.normal = mesh_calculate_terrain_normal(state, x, y);
			vertex.uv = (Vector2f) { x, y };
			array_append(&vertices, &vertex);
		}
	}

	for (int x = 0; x < width - 1; x++) {
		for (int y = 0; y < height - 1; y++) {
			u32 i5 = x + width * y;
			u32 i4 = x + width * (y + 1);
			u32 i3 = (x + 1) + width * y;
			u32 i2 = (x + 1) + width * y;
			u32 i1 = x + width * (y + 1);
			u32 i0 = (x + 1) + width * (y + 1);
			array_append(&indices, &i0);
			array_append(&indices, &i1);
			array_append(&indices, &i2);
			array_append(&indices, &i3);
			array_append(&indices, &i4);
			array_append(&indices, &i5);
		}
	}

	mesh_load_from_memory(
		mesh,
		(Vertex*)vertices.data,
		(u32)vertices.length,
		(u32*)indices.data,
		(u32)indices.length
	);
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
	// I guess the easiest solution would be a switch case with an enum
	// such as VERTEX_POSITION, POSITION_NORMAL, POSITION_COLOR etc.
	// maybe the better solution there would be to remove the middleman and simply have
	// the enum declare alignment e.g. VERTEX_4, VERTEX_4_4_2 etc.
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

void mesh_draw_debug(Mesh* mesh, GLenum mode) {
	vao_bind(&mesh->vao);
	// I pass null here, when I have multiple models, will I need to ensure the ebo is binded again,
	// or is that all just on init, and then I don't have to worry about it again?
	glDrawElements(mode, mesh->indexCount, GL_UNSIGNED_INT, NULL);
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

void mesh_load_cube_custom(Mesh* mesh, u8 width, u8 height, u8 depth) {
	Vertex vertices[] = {
		{{-0.5 * width, -0.5 * height,  0.5 * depth}, {0, 0, 1}, {0, 0}},
		{{ 0.5 * width, -0.5 * height,  0.5 * depth}, {0, 0, 1}, {1, 0}},
		{{ 0.5 * width,  0.5 * height,  0.5 * depth}, {0, 0, 1}, {1, 1}},
		{{-0.5 * width,  0.5 * height,  0.5 * depth}, {0, 0, 1}, {0, 1}},

		{{ 0.5 * width, -0.5 * height, -0.5 * depth}, {0, 0, -1}, {0, 0}},
		{{-0.5 * width, -0.5 * height, -0.5 * depth}, {0, 0, -1}, {1, 0}},
		{{-0.5 * width,  0.5 * height, -0.5 * depth}, {0, 0, -1}, {1, 1}},
		{{ 0.5 * width,  0.5 * height, -0.5 * depth}, {0, 0, -1}, {0, 1}},

		{{-0.5 * width, -0.5 * height, -0.5 * depth}, {-1, 0, 0}, {0, 0}},
		{{-0.5 * width, -0.5 * height,  0.5 * depth}, {-1, 0, 0}, {1, 0}},
		{{-0.5 * width,  0.5 * height,  0.5 * depth}, {-1, 0, 0}, {1, 1}},
		{{-0.5 * width,  0.5 * height, -0.5 * depth}, {-1, 0, 0}, {0, 1}},

		{{ 0.5 * width, -0.5 * height,  0.5 * depth}, {1, 0, 0}, {0, 0}},
		{{ 0.5 * width, -0.5 * height, -0.5 * depth}, {1, 0, 0}, {1, 0}},
		{{ 0.5 * width,  0.5 * height, -0.5 * depth}, {1, 0, 0}, {1, 1}},
		{{ 0.5 * width,  0.5 * height,  0.5 * depth}, {1, 0, 0}, {0, 1}},

		{{-0.5 * width,  0.5 * height,  0.5 * depth}, {0, 1, 0}, {0, 0}},
		{{ 0.5 * width,  0.5 * height,  0.5 * depth}, {0, 1, 0}, {1, 0}},
		{{ 0.5 * width,  0.5 * height, -0.5 * depth}, {0, 1, 0}, {1, 1}},
		{{-0.5 * width,  0.5 * height, -0.5 * depth}, {0, 1, 0}, {0, 1}},

		{{-0.5 * width, -0.5 * height, -0.5 * depth}, {0, -1, 0}, {0, 0}},
		{{ 0.5 * width, -0.5 * height, -0.5 * depth}, {0, -1, 0}, {1, 0}},
		{{ 0.5 * width, -0.5 * height,  0.5 * depth}, {0, -1, 0}, {1, 1}},
		{{-0.5 * width, -0.5 * height,  0.5 * depth}, {0, -1, 0}, {0, 1}},
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

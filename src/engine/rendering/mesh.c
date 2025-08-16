#include "mesh.h"

void mesh_load_from_obj(Mesh* mesh, const char* filePath) {
	u32 positionCount = 0;
	vec3* outPositions = 0;
	u32 normalCount = 0;
	vec3* outNormals = 0;
	u32 uvCount = 0;
	vec2* outUvs = 0;

	u32 positionIndexCount = 0;
	u32* positionIndices = 0;
	u32 normalIndexCount = 0;
	u32* normalIndices = 0;
	u32 uvIndexCount = 0;
	u32* uvIndices = 0;

	FILE* fp = fopen(filePath, "r");
	// TODO! the reallocs should probably use a temp pointer to avoid memory issues
	// TODO! dont think I'm freeing everything I should
	// TODO! and I need to close the file as well
	
	if (!fp) {
		// exit if the we couldn't load a file with the path, likely a typo
		ERROR_EXIT(IO_READ_ERROR_GENERAL, filePath, errno);
	}

	void* tmp;

	while (1) {
		// assume the first word of a line is <= 128 for now
		char lineHeader[128] = {0};
		int res = fscanf(fp, "%s", lineHeader);
		if (res == EOF) {
			break;
		}

		if (strcmp(lineHeader, "v") == 0) {
			vec3 position;
			(void)fscanf(fp, "%f %f %f\n", &position[0], &position[1], &position[2]);
			positionCount++;
			//outPositions = realloc(outPositions, positionCount * sizeof(vec3));
			tmp = realloc(outPositions, positionCount * sizeof(vec3));
			if (!tmp) {
				fclose(fp);
				ERROR_EXIT("Error reallocating memory for mesh initialisation: %s\n", filePath);
			}
			outPositions = tmp;
			memcpy(outPositions[positionCount - 1], &position, sizeof(vec3));
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			vec2 uv;
			(void)fscanf(fp, "%f %f\n", &uv[0], &uv[1]);
			uvCount++;
			tmp = realloc(outUvs, uvCount * sizeof(vec2));
			if (!tmp) {
				fclose(fp);
				ERROR_EXIT("Error reallocating memory for mesh initialisation: %s\n", filePath);
			}
			outUvs = tmp;
			memcpy(outUvs[uvCount - 1], &uv, sizeof(vec2));
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			vec3 normal;
			(void)fscanf(fp, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
			normalCount++;
			tmp = realloc(outNormals, normalCount * sizeof(vec3));
			if (!tmp) {
				fclose(fp);
				ERROR_EXIT("Error reallocation memory for mesh initialisation: %s\n", filePath);
			}
			outNormals = tmp;
			memcpy(outNormals[normalCount - 1], &normal, sizeof(vec3));
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
			// TODO!
			// this is stupid, surely I could get away with a single indexCount right?
			positionIndexCount += 3;
			tmp = realloc(positionIndices, positionIndexCount * sizeof(u32));
			if (!tmp) {
				free(fp);
				ERROR_EXIT("Error reallocating memory for mesh initialisation: %s\n", filePath);
			}
			positionIndices = tmp;
			positionIndices[positionIndexCount - 3] = positionIndex[0];
			positionIndices[positionIndexCount - 2] = positionIndex[1];
			positionIndices[positionIndexCount - 1] = positionIndex[2];
			uvIndexCount += 3;
			tmp = realloc(uvIndices, uvIndexCount * sizeof(u32));
			if (!tmp) {
				free(fp);
				ERROR_EXIT("Error reallocating memory for mesh initialisation: %s\n", filePath);
			}
			uvIndices = tmp;
			uvIndices[uvIndexCount - 3] = uvIndex[0];
			uvIndices[uvIndexCount - 2] = uvIndex[1];
			uvIndices[uvIndexCount - 1] = uvIndex[2];
			normalIndexCount += 3;
			tmp = realloc(normalIndices, normalIndexCount * sizeof(u32));
			if (!tmp) {
				free(fp);
				ERROR_EXIT("Error reallocating memory for mesh initialisation: %s\n", filePath);
			}
			normalIndices = tmp;
			normalIndices[normalIndexCount - 3] = normalIndex[0];
			normalIndices[normalIndexCount - 2] = normalIndex[1];
			normalIndices[normalIndexCount - 1] = normalIndex[2];
		}
	}
	fclose(fp);

	// we now have all the data, need to process it into a pointer array of vertices, and a pointer array
	// of indices, then we can call mesh_load_from_memory
	Vertex* meshVertices = NULL;
	u32 vertexCount = 0;
	u32* meshIndices = NULL;
	u32 indexCount = 0;

	// printf confirmed data is loaded correctly :)
	for (u32 i = 0; i < positionIndexCount; i++) {
		Vertex v = {0};

		u32 positionIndex = positionIndices[i] - 1;
		u32 normalIndex = normalIndices[i] - 1;
		u32 uvIndex = uvIndices[i] - 1;

		memcpy(v.position, outPositions[positionIndex], sizeof(vec3));
		memcpy(v.normal, outNormals[normalIndex], sizeof(vec3));
		memcpy(v.uv, outUvs[uvIndex], sizeof(vec2));

		i32 existingIndex = -1;
		// TODO! really bad performance, looping the entire thing just to check if a vertex is
		// equal, Hash functionality would be much nicer :)
		for (u32 j = 0; j < vertexCount; j++) {
			if (vertices_are_equal(&meshVertices[j], &v)) {
				existingIndex = j;
				break;
			}
		}

		if (existingIndex >= 0) {
			indexCount++;
			tmp = realloc(meshIndices, indexCount * sizeof(u32));
			if (!tmp) {
				ERROR_EXIT("Error reallocating memory for mesh initialisation: %s\n", filePath);
			}
			meshIndices = tmp;
			meshIndices[indexCount - 1] = existingIndex;
		}
		else {
			vertexCount++;
			tmp = realloc(meshVertices, vertexCount * sizeof(Vertex));
			if (!tmp) {
				ERROR_EXIT("Error reallocating memory for mesh initialisation: %s\n", filePath);
			}
			meshVertices = tmp;
			meshVertices[vertexCount - 1] = v;

			indexCount++;
			tmp = realloc(meshIndices, indexCount * sizeof(u32));
			if (!tmp) {
				ERROR_EXIT("Error reallocating memory for mesh initialisation: %s\n", filePath);
			}
			meshIndices = tmp;
			meshIndices[indexCount - 1] = vertexCount - 1;
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

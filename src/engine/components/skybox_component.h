#ifndef SKYBOX_COMPONENT_H
#define SKYBOX_COMPONENT_H

#include "../rendering/mesh.h"
#include "../rendering/texture.h"

typedef struct skybox {
	Mesh cube;
	Texture cubemap;
} Skybox;

Skybox skybox_init(const char** texturePaths);
void skybox_draw(Skybox* s);
void skybox_clean(Skybox* s);

#endif
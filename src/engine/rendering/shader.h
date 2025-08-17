#ifndef SHADER_H
#define SHADER_H

#include "../types.h"
#include "../io/io.h"
#include "../util.h"

#include <linmath.h>
#include <glad/glad.h>
#include <stdio.h>

typedef u32 shader;

shader shader_load(const char* vert_path, const char* frag_path);
void shader_use(shader* id);
void shader_clean(shader* id);

// utils: setting shader uniforms
void shader_set_int(shader* id, const char* name, i32 value);
void shader_set_float(shader* id, const char* name, f32 value);
void shader_set_vec2(shader* id, const char* name, vec2* value);
void shader_set_vec3(shader* id, const char* name, vec3* value);
void shader_set_vec4(shader* id, const char* name, vec4* value);
void shader_set_mat4(shader* id, const char* name, mat4x4* value);

#endif
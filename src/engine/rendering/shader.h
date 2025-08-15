#ifndef SHADER_H
#define SHADER_H

#include "../types.h"
#include "../io/io.h"
#include "../util.h"

#include <linmath.h>
#include <glad/glad.h>
#include <stdio.h>

u32 shader_load(const char* vert_path, const char* frag_path);
void shader_use(u32* id);
void shader_clean(u32* id);

// utils: setting shader uniforms
void shader_set_int(u32* id, const char* name, i32 value);
void shader_set_float(u32* id, const char* name, f32 value);
void shader_set_vec2(u32* id, const char* name, vec2* value);
void shader_set_vec3(u32* id, const char* name, vec3* value);
void shader_set_vec4(u32* id, const char* name, vec4* value);
void shader_set_mat4(u32* id, const char* name, mat4x4* value);

#endif
#include "shader.h"

u32 shader_load(const char* vert_path, const char* frag_path) {
	int success;
	char log[512];

	File file_vertex = io_file_read(vert_path);
	if (!file_vertex.is_valid) {
		ERROR_EXIT("Error reading shader: %s\n", vert_path);
	}

	u32 shader_vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader_vertex, 1, (const char* const*)&file_vertex, NULL);
	glCompileShader(shader_vertex);
	glGetShaderiv(shader_vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader_vertex, 512, NULL, log);
		ERROR_EXIT("Error compiling vertex shader: %s\n", log);
	}

	File file_fragment = io_file_read(frag_path);
	if (!file_fragment.is_valid) {
		ERROR_EXIT("Error reading shader: %s\n", frag_path);
	}

	u32 shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader_fragment, 1, (const char* const*)&file_fragment, NULL);
	glCompileShader(shader_fragment);
	glGetShaderiv(shader_fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader_fragment, 512, NULL, log);
		ERROR_EXIT("Error compiling fragment shader: %s\n", log);
	}

	u32 shader = glCreateProgram();
	glAttachShader(shader, shader_vertex);
	glAttachShader(shader, shader_fragment);
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader, 512, NULL, log);
		ERROR_EXIT("Error linking shader: %s\n", log);
	}

	free(file_vertex.data);
	free(file_fragment.data);

	return shader;
}

void shader_use(u32* id) {
	glUseProgram(*id);
}

void shader_clean(u32* id) {
	glDeleteProgram(*id);
}

// utils: setting uniforms

void shader_set_int(u32* id, const char* name, i32 value) {
	glUniform1i(glGetUniformLocation(*id, name), value);
}

void shader_set_float(u32* id, const char* name, f32 value) {
	glUniform1f(glGetUniformLocation(*id, name), value);
}

void shader_set_vec2(u32* id, const char* name, vec2* value) {
	glUniform2fv(glGetUniformLocation(*id, name), 1, value[0]);
}

void shader_set_vec3(u32* id, const char* name, vec3* value) {
	glUniform3fv(glGetUniformLocation(*id, name), 1, value[0]);
}
void shader_set_vec4(u32* id, const char* name, vec4* value) {
	glUniform4fv(glGetUniformLocation(*id, name), 1, value[0]);
}

void shader_set_mat4(u32* id, const char* name, mat4x4* value) {
	glUniformMatrix4fv(glGetUniformLocation(*id, name), 1, GL_FALSE, value[0][0]);
}

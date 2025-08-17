#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;

out vs_out {
	vec2 frag_uv;
	float frag_light_intensity;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//TODO! would like actual lighting at some point, hardcoding it here aint great
const vec3 LIGHT_DIRECTION = normalize(vec3(-6.0, 10.0, 4.0));

void main() {
	vec3 normal_world_space = normalize(mat3(model) * a_normal);
	float light_intensity = max(dot(normal_world_space, LIGHT_DIRECTION), 0);

	gl_Position = projection * view * model * vec4(a_pos, 1.0);
	vs_out.frag_uv = a_uv;
	vs_out.frag_light_intensity = light_intensity;
}
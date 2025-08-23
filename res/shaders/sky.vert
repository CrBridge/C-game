#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;

out vec3 frag_uv;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    frag_uv = a_pos;
	mat4 view_no_translation = mat4(mat3(view));
    gl_Position = projection * view_no_translation * vec4(a_pos, 1.0);
}  
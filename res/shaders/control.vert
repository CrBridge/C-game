#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;

out vec2 frag_uv;

uniform mat4 projection;

void main() 
{
	gl_Position = projection * vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);
	frag_uv = a_uv;
}
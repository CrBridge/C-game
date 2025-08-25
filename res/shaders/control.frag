#version 460 core

in vec2 frag_uv;

out vec4 frag_color;

uniform sampler2D texture_id;

void main() 
{
	frag_color = texture(texture_id, frag_uv);
}
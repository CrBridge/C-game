#version 460 core

in vec2 frag_uv;
in vec3 frag_color;

out vec4 out_color;

uniform sampler2D texture_id;

void main() 
{
	out_color = vec4(frag_color, 1.0) * texture(texture_id, frag_uv);
}
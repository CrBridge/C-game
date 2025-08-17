#version 460 core

in vec2 frag_uv;

out vec4 frag_color;

uniform sampler2D texture_id;

void main()
{ 
	//TODO! the screen quad might not even need uv, I might
	//	be able to use the builtin gl_FragCoord to calculate output
	//	but i'd have to benchmark, the quad is such a small amount
	//	of memory and doing this calculation would probably be detrimental
    frag_color = texture(texture_id, frag_uv);
}
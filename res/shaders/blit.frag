#version 460 core

in vec2 frag_uv;

out vec4 frag_color;

uniform sampler2D texture_id;

uniform int frame;

float hash12(vec2 p) {
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return fract(p.x * p.y);
}

void main()
{ 
	frag_color = texture(texture_id, frag_uv);

	// messing about with a film grain effect 
    //vec4 color = texture(texture_id, frag_uv);
	//float grain = hash12(frag_uv + float(frame));
	//grain = (grain * 2 - 1) * 0.2;
	//frag_color = color + color * grain;
}
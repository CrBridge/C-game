#version 460 core

in vec2 frag_uv;

out vec4 out_color;

uniform sampler2D texture_id;

uniform int frame;

float hash12(vec2 p) {
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return fract(p.x * p.y);
}

void main()
{ 
	// messing about with a film grain effect
    vec4 color = texture(texture_id, frag_uv);
	float grain = hash12(frag_uv + float(frame));
	grain = (grain * 2 - 1) * 0.2;
	vec4 grain_only = color + color * grain;
	out_color = grain_only;
}
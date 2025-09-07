#version 460 core

in vs_out {
	vec2 frag_uv;
	float frag_light_intensity;
} fs_in;

out vec4 out_color;

uniform sampler2D texture_id;

uniform float time;

//TODO! this doesn't affect the directional light colour, only the ambient
const vec3 LIGHT_COLOUR = vec3(1.0);

void main() {
	vec4 texture_col = texture(texture_id, fs_in.frag_uv);

	vec3 ambient_light = LIGHT_COLOUR * 0.1;
	vec3 lit_result = (ambient_light + fs_in.frag_light_intensity) * texture_col.xyz;

	out_color = vec4(lit_result, texture_col.a);
}
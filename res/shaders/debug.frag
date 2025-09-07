#version 460 core

in vs_out {
	vec2 frag_uv;
} fs_in;

out vec4 out_color;

uniform float time;

void main() {
	vec4 col = vec4(1);
	col.xz *= sin(time * 0.2);

	out_color = col;
}
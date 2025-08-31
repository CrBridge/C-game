#version 460 core

in vs_out {
	vec2 frag_uv;
	vec4 frag_relative_pos;
} fs_in;

out vec4 out_color;

uniform sampler2D texture_id;

uniform float threshold;

float hash12(vec2 p) {
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return fract(p.x * p.y);
}

float map(float v, float min_in, float max_in, float min_out, float max_out) {
	return min_out + (max_out - min_out) * (v - min_in) / (max_in - min_in);
}

// If i dont want it darker further away, the best bet I think is fog
// of the same colour, but that wont fix the noisy jittering issue on
// the quad edge unless I change the skybox, so alot to chew on here

void main() {
    vec2 gridUV = floor(fs_in.frag_uv * 400);

    float rand = hash12(gridUV);
	// linear map rand to make our job easier, will go in the range of number of shells / 10 
	// in this case there are 16 shells so we map to 1.6
	rand = map(rand, 0.0, 1.0, 0.0, 1.6);

    if (rand < threshold) {
        discard;
    }

	vec4 texture_color = texture(texture_id, fs_in.frag_uv);

	// multiply by threshold for depth
	//frag_color = vec4(texture_color.xyz * threshold / 1.6, 1.0);

	// for context in case I remove it frag_relative_pos is view*model*a_pos
	// this line causes grass to be darker the further it is away. Looks really cool imo for
	// some kind of night time effect, but maybe I can calculate it in the vertex instead?
	float relative_position_length = length(fs_in.frag_relative_pos);
	relative_position_length = clamp(relative_position_length, 0, 100.0);
	out_color = vec4((texture_color.xyz * threshold / 1.6) * (1 / relative_position_length), 1.0);
}
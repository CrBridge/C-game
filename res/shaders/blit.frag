#version 460 core

in vec2 frag_uv;

out vec4 out_color;

uniform sampler2D texture_id;

uniform int frame;

const int bayer2[2 * 2] = {
	0, 2,
	3, 1
};

const int bayer4[4 * 4] = {
	0, 8, 2, 10,
    12, 4, 14, 6,
    3, 11, 1, 9,
    15, 7, 13, 5
};

const int bayer8[8 * 8] = {
    0, 32, 8, 40, 2, 34, 10, 42,
    48, 16, 56, 24, 50, 18, 58, 26,  
    12, 44,  4, 36, 14, 46,  6, 38, 
    60, 28, 52, 20, 62, 30, 54, 22,  
    3, 35, 11, 43,  1, 33,  9, 41,  
    51, 19, 59, 27, 49, 17, 57, 25, 
    15, 47,  7, 39, 13, 45,  5, 37, 
    63, 31, 55, 23, 61, 29, 53, 21
};


float get_bayer2(int x, int y) {
	return float(bayer2[(x % 2) + (y % 2) * 2]) * (1.0f / 4.0f) - 0.5f;
}

float get_bayer4(int x, int y) {
	return float(bayer4[(x % 4) + (y % 4) * 4]) * (1.0f / 16.0f);
}

float get_bayer8(int x, int y) {
	return float(bayer8[(x % 8) + (y % 8) * 8]) * (1.0f / 64.0f) - 0.5;
}

float hash12(vec2 p) {
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return fract(p.x * p.y);
}

vec4 sharpen(in sampler2D tex, in vec2 coords, in vec2 renderSize) {
  float dx = 1.0 / renderSize.x;
  float dy = 1.0 / renderSize.y;
  vec4 sum = vec4(0.0);
  sum += -1. * texture2D(tex, coords + vec2( -1.0 * dx , 0.0 * dy));
  sum += -1. * texture2D(tex, coords + vec2( 0.0 * dx , -1.0 * dy));
  sum += 5. * texture2D(tex, coords + vec2( 0.0 * dx , 0.0 * dy));
  sum += -1. * texture2D(tex, coords + vec2( 0.0 * dx , 1.0 * dy));
  sum += -1. * texture2D(tex, coords + vec2( 1.0 * dx , 0.0 * dy));
  return sum;
}

void main()
{ 
	out_color = texture(texture_id, frag_uv);

	// sharpening, would really want to pass in resolution as a uniform instead
	//vec4 sharpened = sharpen(texture_id, frag_uv, vec2(960,544));
	//out_color = sharpened;

	// messing about with a film grain effect
    //vec4 color = texture(texture_id, frag_uv);
	//float grain = hash12(frag_uv + float(frame));
	//grain = (grain * 2 - 1) * 0.2;
	//vec4 grain_only = color + color * grain;
	//out_color = grain_only;
	//vec4 grain_sharpen = sharpened + sharpened * grain;
	//out_color = grain_sharpen;

	// trying dithering, needs some more tinkering
	//int x = int(frag_uv.x * 960);
	//int y = int(frag_uv.y * 544);
	//
	//vec3 bayer = { 0, 0, 0 };
	//bayer.x = get_bayer2(x, y);
	//bayer.y = get_bayer4(x, y);
	//bayer.z = get_bayer8(x, y);
	//
	//vec4 color = texture(texture_id, frag_uv);
	//out_color = color + 0.1 * bayer[0];
}
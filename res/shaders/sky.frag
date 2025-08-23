#version 460 core

in vec3 frag_uv;

out vec4 frag_color;

uniform samplerCube skybox;

uniform float time;

float hash13(vec3 p3)
{
	p3  = fract(p3 * .1031);
	p3 += dot(p3, p3.zyx + 31.32);
	return fract((p3.x + p3.y) * p3.z);
}

void main()
{
    frag_color = texture(skybox, frag_uv);
	//frag_color = vec4(vec3(hash13(vec3(texture(skybox, frag_uv)))), 1.0);
}  
#version 460 core

in vec3 frag_uv;

out vec4 out_color;

uniform samplerCube skybox;

void main()
{
    out_color = texture(skybox, frag_uv);
}  
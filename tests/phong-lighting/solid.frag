#version 420 core

in vec3 vcolor;

out vec4 out_frag_color;

void main()
{
    out_frag_color = vec4(vcolor, 1.0f);
} 

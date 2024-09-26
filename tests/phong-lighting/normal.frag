#version 150 core

in vec3 vnormal;

out vec4 out_color;

void main()
{
	out_color = vec4(vnormal * 0.5 + 0.5, 1.0);
} 

#version 420 core

in vec3 pos;
in vec3 norm;

out vec3 vcolor;

uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;
uniform vec3 color;

void main()
{
    gl_Position = proj * view * model * vec4(pos, 1.0);
	vcolor = color;
}

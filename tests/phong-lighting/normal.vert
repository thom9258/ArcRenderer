#version 150 core

in vec3 in_position;
in vec3 in_normal;

out vec3 vnormal;

uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;
uniform vec3 color;

void main()
{
    gl_Position = proj * view * model * vec4(in_position, 1.0);
	vnormal = in_normal;
}

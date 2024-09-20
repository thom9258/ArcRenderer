#version 150 core
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;

layout (location = 0) out vec3 outColor;

uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;
uniform vec3 color;

void main()
{
    gl_Position = proj * view * model * vec4(pos, 1.0);
	outColor = color;
}

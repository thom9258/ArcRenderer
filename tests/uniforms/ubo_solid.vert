#version 150 core
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;

layout (location = 0) out vec3 outColor;

layout (std140) uniform Camera {
    mat4 view;
    mat4 proj;
}camera;

layout (std140) uniform Obj {
    mat4 model;
    vec3 color;
}obj;

void main()
{
    gl_Position = camera.proj * camera.view * obj.model * vec4(pos, 1.0);
	outColor = obj.color;
}

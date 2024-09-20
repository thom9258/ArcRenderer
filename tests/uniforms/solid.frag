#version 150 core
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inColor;

out vec4 outFragColor;

void main()
{
    outFragColor = vec4(inColor, 1.0f);
} 

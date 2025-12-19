#version 460

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iNorm;
layout(location = 2) in vec2 iTexcoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(iPos, 1.f);
}

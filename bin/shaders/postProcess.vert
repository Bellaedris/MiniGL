#version 460

layout(location = 0) in vec3 iPos;
layout(location = 2) in vec2 iTexcoord;

out vec2 texcoord;

void main() {
    texcoord = iTexcoord;
    gl_Position = vec4(iPos, 1.);
}

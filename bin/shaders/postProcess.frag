#version 460

in vec2 texcoord;

out vec4 FragColor;

uniform sampler2D framebuffer;

void main() {
    FragColor = texture(framebuffer, texcoord);
}
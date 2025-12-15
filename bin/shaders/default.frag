#version 460

in vec3 normal;
in vec2 texcoord;

out vec4 FragColor;

void main() {
    vec3 lightDir = normalize(vec3(0, -1, -1));
    float cosTheta = max(.0f, dot(normal, -lightDir));
    vec3 color = cosTheta * vec3(1, 1, 1);

    FragColor = vec4(color, 1.0);
}

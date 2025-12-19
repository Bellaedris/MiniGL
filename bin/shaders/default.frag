#version 460

in vec3 normal;
in vec2 texcoord;
in vec4 positionLightspace;

out vec4 FragColor;

uniform sampler2D albedo;
uniform sampler2D shadowmap;

void main() {
    vec3 lightDir = normalize(vec3(0, -1, -1));
    float cosTheta = max(.0f, dot(normal, -lightDir));
    vec3 color = cosTheta * texture(albedo, texcoord).xyz;

    vec3 posLight = positionLightspace.xyz / positionLightspace.w;
    posLight = (posLight + 1.f) * .5f;
    vec2 imgRes = textureSize(shadowmap, 0);
    vec2 texelSize = 1.f / imgRes;
    float totalShadow = .0f;

    for(int j = -1; j <= 1; j++)
    {
        for(int i = -1; i <= 1; i++)
        {
            float depth = texture(shadowmap, posLight.xy + vec2(i, j) * texelSize).r;
            if(posLight.z > depth + 0.0001f)
            {
                totalShadow += 1.f;
            }
        }
    }
    totalShadow /= 9.f;
    color = color * mix(1, 0, totalShadow);

    FragColor = vec4(color, 1.0);
}
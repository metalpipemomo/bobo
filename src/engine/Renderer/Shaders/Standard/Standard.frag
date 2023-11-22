#version 410

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

uniform sampler2D texSampler;

void main()
{
    outColor = texture(texSampler, fragTexCoord);
}
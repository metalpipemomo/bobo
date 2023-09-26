#version 430

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;

out vec2 TexCoord;
out vec3 Normal;

uniform mat4 mvp;
uniform mat3 nm;

void main()
{
    TexCoord = inTexCoord;
    Normal = normalize(nm * inNormal);

    gl_Position = mvp * vec4(inPosition, 1.0);
}
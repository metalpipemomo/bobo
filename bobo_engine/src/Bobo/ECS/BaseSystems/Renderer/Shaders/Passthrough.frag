#version 430
precision mediump float;

in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D tex;

out vec4 Color;

void main()
{
    vec4 color = texture(tex, TexCoord);
    Color = color;
}
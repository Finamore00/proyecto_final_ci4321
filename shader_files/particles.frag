#version 420 core

in vec4 Color;
in vec2 TexCoord;

layout (binding = 0) uniform sampler2D base;

out vec4 FragColor;

void main()
{
    FragColor = texture(base, TexCoord) * Color;
}
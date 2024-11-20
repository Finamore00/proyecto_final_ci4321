#version 420 core

in vec2 TexCoord;

uniform sampler2D sprite;

out vec4 FragColor;

void main()
{
    FragColor = texture(sprite, TexCoord);
}
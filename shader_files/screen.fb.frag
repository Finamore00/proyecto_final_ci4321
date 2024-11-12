#version 420 core

in vec2 TexCoord;

uniform sampler2D screen;

out vec4 FragColor;

void main()
{
    FragColor = texture(screen, TexCoord);
}
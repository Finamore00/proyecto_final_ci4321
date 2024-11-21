#version 420 core

in vec2 TexCoord;
uniform vec2 sizes;
uniform vec2 charOffset;

uniform sampler2D font;

out vec4 FragColor;

void main()
{
    // Quitar hardcodeo
    FragColor = texture(font, TexCoord * vec2(1.0/16.0, 1.0/8.0) + charOffset);
}

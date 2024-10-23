#version 400 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 vNor;

uniform vec4 fColor;
uniform sampler2D base;

void main()
{
   FragColor = texture(base, TexCoord) + fColor;
}

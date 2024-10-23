#version 400 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 vColor;
in vec3 vNor;

uniform sampler2D ourTexture;

void main()
{
   FragColor = texture(ourTexture, TexCoord);
   FragColor = vec4(vColor, 1.0);
}

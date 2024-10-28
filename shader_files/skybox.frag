#version 420 core

in vec3 TexCoords;

uniform samplerCube skybox;

out vec4 FragColor;

void main()
{    
    FragColor = texture(skybox, vec3(TexCoords.x, -TexCoords.y, TexCoords.z));
}
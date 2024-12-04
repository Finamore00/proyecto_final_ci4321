#version 420 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec4 aColor;
layout (location = 3) in mat4 aModel;

layout (std140, binding = 0) uniform Matrices
{
                        // base  // offset
    mat4 projection;    // 16    // 0
    mat4 view;          // 16    // 64 (16 * 4)
};

out vec2 TexCoord;
out vec4 Color;

void main()
{
    
}
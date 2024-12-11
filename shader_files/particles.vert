#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUVs;
layout (location = 2) in float aSize;
layout (location = 3) in vec4 aColor;
layout (location = 4) in mat4 aModel;

layout (std140, binding = 0) uniform Matrices
{
                        // base  // offset
    mat4 projection;    // 16    // 0
    mat4 view;          // 16    // 64 (16 * 4)
};

out vec4 Color;
out vec2 TexCoord;

void main()
{
    mat4 MVM = view * aModel;
    MVM[1][0] = MVM[2][0] = MVM[0][1] = MVM[2][1] = MVM[0][2] = MVM[1][2] = 0;
    MVM[0][0] = MVM[1][1] = MVM[2][2] = aSize;
    gl_Position = projection * MVM * vec4(aPos, 1.0);
    
    TexCoord = aUVs;
    Color = aColor;
}
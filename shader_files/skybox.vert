#version 420 core
layout (location = 0) in vec3 aPos;
layout (std140, binding = 0) uniform Matrices
{
                        // base  // offset
    mat4 projection;    // 16    // 0
    mat4 view;          // 16    // 64 (16 * 4)
};

out vec3 TexCoords;

void main()
{
   TexCoords = aPos;
   gl_Position = projection * view * vec4(aPos, 1.0);
}

#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec2 aTexCoord;
layout (std140, binding = 0) uniform Matrices
{
                        // base  // offset
    mat4 projection;    // 16    // 0
    mat4 view;          // 16    // 64 (16 * 4)
};

uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   FragPos = vec3(model * vec4(aPos, 1.0));
   //Normal = aNor;
   //mat3 normalMatrix = transpose(inverse(mat3(model)));
   //Normal = normalize(normalMatrix * aNor);
   Normal = vec3(model * vec4(aNor, 0.0));
   //Normal = mat3(transpose(inverse(model))) * aNor;
   TexCoord = aTexCoord;
}

#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (std140, binding = 0) uniform Matrices
{
                        // base  // offset
    mat4 projection;    // 16    // 0
    mat4 view;          // 16    // 64 (16 * 4)
};

uniform mat4 model;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out mat3 TBN;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   FragPos = vec3(model * vec4(aPos, 1.0));
   vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
   vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
   vec3 N = normalize(vec3(model * vec4(aNor, 0.0)));
   TBN = mat3(T, B, N);
   TexCoord = aTexCoord;
   Normal = vec3(model * vec4(aNor, 0.0));
}

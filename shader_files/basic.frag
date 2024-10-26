#version 420 core
layout (std140, binding = 1) uniform Lights
{
                  // base  // offset
   vec3 l1_pos;   // 16    // 0
   vec3 l1_col;   // 16    // 16
   float l1_pow;  // 4     // 32
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

struct Light
{
   vec3  pos;
   vec3  col;
   float power;
};

struct UVT
{
   vec2 uvOffset;
   vec2 uvScale;
};

uniform vec3 tint;
uniform Light light;
uniform vec3 ambient;
uniform sampler2D base;
uniform UVT uvt;

out vec4 FragColor;

void main()
{
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(l1_pos - FragPos);
   float factor = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = factor * l1_pow * l1_col;

   FragColor = texture(base, TexCoord * uvt.uvScale + uvt.uvOffset) * (vec4(diffuse, 1.0) + vec4(ambient, 1.0)) * vec4(tint, 1.0);
}

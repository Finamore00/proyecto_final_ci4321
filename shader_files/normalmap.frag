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
uniform vec3 camPos;
uniform sampler2D base;
uniform UVT uvt;

out vec4 FragColor;

void main()
{
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(l1_pos - FragPos);
   float factor = max(dot(norm, lightDir), 0.0) * l1_pow;
   vec3 diffuse = factor * l1_col;

   float specularStrength = 0.5;
   vec3 viewDir = normalize(camPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = specularStrength * spec * l1_col * 4.0;

   FragColor = ((texture(base, TexCoord * uvt.uvScale + uvt.uvOffset) + vec4(tint, 1.0)) * vec4(diffuse + ambient + specular, 1.0));
}

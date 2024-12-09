#version 420 core

#define MAX_LIGHT_CAPACITY 128


struct Light
{
   vec3  pos;
   vec3  col;
   vec3  dir;
   int   type;
   float cutoff;
   float power;
};

layout (std140, binding = 1) uniform Lights {
   Light lights[MAX_LIGHT_CAPACITY];
   int light_count;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

struct UVT
{
   vec2 uvOffset;
   vec2 uvScale;
};

uniform vec3 tint;
uniform vec3 ambient;
uniform sampler2D base;
uniform UVT uvt;

out vec4 FragColor;

void main()
{
   vec3 result = vec3(0.0);
   vec3 norm = normalize(Normal);

   for (int i = 0; i < light_count; i++) {
      vec3 l_pos = lights[i].pos;
      vec3 l_col = lights[i].col;
      float l_pow = lights[i].power;
      vec3 light_dir = normalize(l_pos - FragPos);
      float factor = max(dot(norm, light_dir), 0.0) * l_pow;
      vec3 diffuse = factor * l_col;
      result += diffuse;
   }

   FragColor = ((texture(base, TexCoord * uvt.uvScale + uvt.uvOffset) + vec4(tint, 1.0)) * vec4(result + ambient, 1.0));
}

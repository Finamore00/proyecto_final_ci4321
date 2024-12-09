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

layout (std140, binding = 1) uniform Lights
{
   Light lights[MAX_LIGHT_CAPACITY];
   int light_count;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in mat3 TBN;

struct UVT
{
   vec2 uvOffset;
   vec2 uvScale;
};

uniform vec3 tint;
uniform Light light;
uniform vec3 ambient;
uniform vec3 camPos;
layout (binding = 0) uniform sampler2D base;
layout (binding = 1) uniform sampler2D normalMap;
uniform UVT uvt;

out vec4 FragColor;

void main()
{

   vec3 result = vec3(0.0);

   vec2 uv = TexCoord * uvt.uvScale + uvt.uvOffset;
   vec3 normal = texture(normalMap, uv).rgb;
   normal = normal * 2.0 - 1.0;
   normal = normalize(TBN * normal);

   float specularStrength = 0.5;
   vec3 viewDir = normalize(camPos - FragPos);

   for (int i = 0; i < light_count; i++) {
      vec3 l_pos = lights[i].pos;
      vec3 l_col = lights[i].col;
      float l_pow = lights[i].power;

      vec3 light_dir = normalize(l_pos - FragPos);
      float normal_factor = max(dot(Normal, light_dir), 0.0);
      float factor = max(dot(normal, light_dir), 0.0) * l_pow;
      vec3 diffuse = factor * l_col;

      vec3 reflect_dir = reflect(-light_dir, normal);
      float spec = pow(max(dot(viewDir, reflect_dir), 0.0), 128) * l_pow;
      spec = spec * step(0.0, -step(normal_factor, 0.0));
      vec3 specular = specularStrength * spec * l_col;

      result += diffuse + specular;
   }

   FragColor = ((texture(base, uv) + vec4(tint, 1.0)) * vec4(ambient + result, 1.0));
}

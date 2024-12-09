#version 420 core

#define MAX_LIGHT_CAPACITY 128
#define POINT_LIGHT_ENUM 0
#define DIRECTIONAL_LIGHT_ENUM 1
#define SPOT_LIGHT_ENUM 2


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

vec3 calculate_directional_light(Light light) {
   vec3 light_dir = normalize(-light.dir);
   vec3 norm = normalize(Normal);
   vec3 pos = light.pos;
   vec3 col = light.col;
   float power = light.power;
   float factor = max(dot(norm, light_dir), 0.0) * power;
   vec3 diffuse = factor * col;

   return diffuse;
}

vec3 calculate_point_light(Light light) {
   vec3 light_dir = normalize(light.pos - FragPos);
   vec3 norm = normalize(Normal);
   float factor = max(dot(norm, light_dir), 0.0) * light.power;
   vec3 diffuse = factor * light.col;

   return diffuse;
}

vec3 calculate_spot_light(Light light) {
   vec3 light_dir = normalize(light.pos - FragPos);
   vec3 norm = normalize(Normal);
   float theta = dot(light_dir, normalize(-light.dir));
   float factor = max(dot(norm, light_dir), 0.0) * light.power;
   vec3 diffuse = factor * light.col;

   diffuse *= step(light.cutoff, theta);
   return diffuse;
}

void main()
{
   vec3 result = vec3(0.0);
   vec3 norm = normalize(Normal);

   for (int i = 0; i < light_count; i++) {
      result += calculate_directional_light(lights[i]) * step(DIRECTIONAL_LIGHT_ENUM, lights[i].type) * step(lights[i].type, DIRECTIONAL_LIGHT_ENUM);
      result += calculate_point_light(lights[i]) * step(POINT_LIGHT_ENUM, lights[i].type) * step(lights[i].type, POINT_LIGHT_ENUM);
      result += calculate_spot_light(lights[i]) * step(SPOT_LIGHT_ENUM, lights[i].type) * step(lights[i].type, SPOT_LIGHT_ENUM);
   }

   FragColor = ((texture(base, TexCoord * uvt.uvScale + uvt.uvOffset) + vec4(tint, 1.0)) * vec4(result + ambient, 1.0));
}
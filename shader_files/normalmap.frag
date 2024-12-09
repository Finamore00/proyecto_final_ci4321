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

vec3 calculate_directional_light(Light light, vec3 normal, vec3 view_dir) {
   vec3 light_dir = normalize(-light.dir);
   float normal_factor = max(dot(Normal, light_dir), 0.0);
   float factor = max(dot(normal, light_dir), 0.0) * light.power;
   vec3 diffuse = factor * light.col;

   vec3 reflect_dir = reflect(-light_dir, normal);
   float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 128) * light.power;
   spec = spec * step(0.0, -step(normal_factor, 0.0));
   vec3 specular = 0.5 * spec * light.col;

   return diffuse + specular;
}

vec3 calculate_point_light(Light light, vec3 normal, vec3 view_dir) {
   vec3 light_dir = normalize(light.pos - FragPos);
   float normal_factor = max(dot(Normal, light_dir), 0.0);
   float factor = max(dot(normal, light_dir), 0.0) * light.power;
   vec3 diffuse = factor * light.col;

   vec3 reflect_dir = reflect(-light_dir, normal);
   float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 128) * light.power;
   spec = spec * step(0.0, -step(normal_factor, 0.0));
   vec3 specular = 0.5 * spec * light.col;

   return diffuse + specular;
}

vec3 calculate_spot_light(Light light, vec3 normal, vec3 view_dir) {
   vec3 light_dir = normalize(light.pos - FragPos);
   float normal_factor = max(dot(Normal, light_dir), 0.0);
   float factor = max(dot(normal, light_dir), 0.0) * light.power;
   vec3 diffuse = factor * light.col;

   vec3 reflect_dir = reflect(-light_dir, normal);
   float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 128) * light.power;
   spec = spec * step(0.0, -step(normal_factor, 0.0));
   vec3 specular = 0.5 * spec * light.col;

   float theta = dot(light_dir, normalize(-light.dir));
   return (diffuse + specular) * step(light.cutoff, theta);
}

void main()
{
   vec2 uv = TexCoord * uvt.uvScale + uvt.uvOffset;
   vec3 normal = texture(normalMap, uv).rgb;
   normal = normal * 2.0 - 1.0;
   normal = normalize(TBN * normal);

   float specularStrength = 0.5;
   vec3 viewDir = normalize(camPos - FragPos);

   vec3 result = vec3(0.0);

   for (int i = 0; i < light_count; i++) {
      result += calculate_point_light(lights[i], normal, viewDir) * step(POINT_LIGHT_ENUM, lights[i].type) * step(lights[i].type, POINT_LIGHT_ENUM);
      result += calculate_directional_light(lights[i], normal, viewDir) * step(DIRECTIONAL_LIGHT_ENUM, lights[i].type) * step(lights[i].type, DIRECTIONAL_LIGHT_ENUM);
      result += calculate_spot_light(lights[i], normal, viewDir) * step(SPOT_LIGHT_ENUM, lights[i].type) * step(lights[i].type, SPOT_LIGHT_ENUM);
   }

   FragColor = ((texture(base, uv) + vec4(tint, 1.0)) * vec4(ambient + result, 1.0));
}

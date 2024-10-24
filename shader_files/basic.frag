#version 400 core
struct Light
{
   vec3  pos;
   vec3  col;
   float power;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 tint;
uniform Light light;
uniform vec3 ambient;
uniform sampler2D base;

out vec4 FragColor;

void main()
{
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(light.pos - FragPos);
   float factor = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = factor * light.power * light.col;

   FragColor = texture(base, TexCoord) * (vec4(diffuse, 1.0) + vec4(ambient, 1.0)) * vec4(tint, 1.0);
}

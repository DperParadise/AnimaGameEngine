#version 330

struct Material
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
  sampler2D texture_diffuse1;
};

in vec2 outTexCoord;
out vec4 fragColor;

uniform Material material;


void main()
{
  vec3 color = material.diffuse;
  fragColor = vec4(color, 1.0) + vec4(material.ambient, 1.0);
}
#version 330

struct Material
{
  sampler2D texture_diffuse1;
};

in vec2 outTexCoord;
out vec4 fragColor;

uniform Material material;

void main()
{
  fragColor = vec4(texture(material.texture_diffuse1, outTexCoord).rgb, 1.0);
}
#version 330
layout (location=0) in vec3 position;
layout (location=1) in vec2 texCoord;

out vec2 outTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  outTexCoord = texCoord;
  gl_Position = projection * view * model * vec4(position, 1.0);
}
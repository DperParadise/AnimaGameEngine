#version 330
layout (location=0) in vec3 position;
layout (location=1) in vec3 color;

out vec3 outColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  outColor = color;
  vec4 position = projection * view * model * vec4(position, 1.0);
  //position.z = 0.0;
  gl_Position = position;
}
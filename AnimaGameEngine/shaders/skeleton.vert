#version 330

layout (location=0) in vec3 position;

uniform mat4 view;
uniform mat4 projection;

void main()
{
  vec4 position = projection * view * vec4(position, 1.0);
  position.z = 0.0;
  gl_PointSize = 5.0;
  gl_Position = position;
}
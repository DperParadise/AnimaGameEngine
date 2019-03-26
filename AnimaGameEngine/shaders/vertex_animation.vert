#version 330
layout (location=0) in vec3 position;
layout (location=1) in vec2 texCoord;
layout (location=5) in uvec4 boneIds;
layout (location=6) in vec4 weights;

out vec2 outTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

#define MAX_BONES 100
uniform mat4 bones[MAX_BONES];

void main()
{
  outTexCoord = texCoord;
/*
  mat4 skinningMatrix = bones[boneIds[0]] * weights[0];
  skinningMatrix += bones[boneIds[1]] * weights[1];
  skinningMatrix += bones[boneIds[2]] * weights[2];
  skinningMatrix += bones[boneIds[3]] * weights[3];
*/ 
  gl_Position = projection * view * model * /* skinningMatrix * */ vec4(position, 1.0);
}
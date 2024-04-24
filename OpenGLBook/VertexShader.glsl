#version 400 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 normal;

out vec2 toFragShaderTexCoord;
out vec4 toFragShaderColor;
out vec3 toFragShaderNormal;
out vec3 toFragShaderPos;
out mat4 toFragShaderModel;
out mat4 toFragShaderRotation;

uniform mat4 model;
uniform mat4 rotation;
uniform mat4 view;
uniform mat4 projection;


void main()
{
  gl_Position = projection * view * model * rotation * vec4(pos, 1.0F);
  
  toFragShaderColor    = color;
  toFragShaderTexCoord = texCoord;
  toFragShaderNormal   = normal;
  toFragShaderPos      = pos;
  toFragShaderModel    = model;
  toFragShaderRotation = rotation;
}
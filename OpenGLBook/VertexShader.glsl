#version 400 core

layout (location = 0) in vec3 pos;

uniform mat4 mvp;

out vec4 toFragShaderColor;

void main()
{
  gl_Position = mvp * vec4(pos, 1.0F);
}
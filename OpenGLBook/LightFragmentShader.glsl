#version 400 core

in vec4 toFragShaderColor;
out vec4 outColor;

void main()
{
  outColor = toFragShaderColor;
}
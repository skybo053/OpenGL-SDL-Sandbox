# version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec4 toFragShaderColor;

uniform mat4 mvp;


void main()
{
  toFragShaderColor = color;
  gl_Position       = mvp * vec4(position, 1.0F);
}
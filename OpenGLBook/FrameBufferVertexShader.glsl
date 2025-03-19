# version 400 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec2 inTexCoords;

out vec4 toFragShaderColor;
out vec2 toFragShaderTexCoords;


void main()
{
  toFragShaderColor     = inColor;
  toFragShaderTexCoords = inTexCoords;  
  gl_Position           = vec4(inPosition, 1.0F);
}
#version 400 core


in vec2 toFragShaderTexCoord;
in vec4 toFragShaderColor;

out vec4 outputColor;

uniform sampler2D sampler;


void main()
{
  outputColor = texture(sampler, toFragShaderTexCoord);
  //outputColor = toFragShaderColor;
}
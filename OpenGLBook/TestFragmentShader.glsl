#version 400 core

in vec2 toFragShaderTexCoords;

out vec4 finalColor;

uniform sampler2D sampler;


void main()
{
  finalColor = vec4(1.0F, 0.0F, 0.0F, 1.0F);
  //finalColor = texture(sampler, toFragShaderTexCoords);
}
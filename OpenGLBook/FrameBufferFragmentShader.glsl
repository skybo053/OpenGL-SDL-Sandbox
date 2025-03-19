# version 400 core

in vec4 toFragShaderColor;
in vec2 toFragShaderTexCoords;

out vec4 color;

uniform sampler2D sampler;

void main()
{
  color = texture(sampler, toFragShaderTexCoords);
  //color = vec4(1.0F, 0.0F, 0.0F, 1.0F);
}
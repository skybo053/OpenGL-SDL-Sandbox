#version 400 core


in vec2 toFragShaderTexCoord;
in vec4 toFragShaderColor;
in float toFragShaderDefuseValue;
         

out vec4 outputColor;

uniform sampler2D sampler;


void main()
{
  outputColor = texture(sampler, toFragShaderTexCoord) * toFragShaderDefuseValue;
  //outputColor = vec4(1.0F, 1.0F, 1.0F, 1.0F);
}
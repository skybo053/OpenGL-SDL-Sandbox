#version 400 core


in vec2 toFragShaderTexCoord;
in vec4 toFragShaderColor;
in vec3 toFragShaderNormal;
in vec3 toFragShaderPos; 

out vec4 outputColor;

uniform sampler2D sampler;

uniform mat4 fragModel;
uniform mat4 fragRotation;
uniform vec3 lightPosition;

void main()
{
  vec4 position              = fragModel * fragRotation * vec4(toFragShaderPos, 1.0F);
  vec4 rotatedNormal         = fragRotation * vec4(toFragShaderNormal, 1.0F);
  vec4 vectorToLightPosition = normalize(vec4(lightPosition, 1.0F) - position);
  
  float diffuseValue         = max(dot(vectorToLightPosition, rotatedNormal), 0.0F);
  float ambientValue         = 0.1F;
  
  

  outputColor = texture(sampler, toFragShaderTexCoord) * (diffuseValue + ambientValue);
  //outputColor = vec4(1.0F, 1.0F, 1.0F, 1.0F);
}
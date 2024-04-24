#version 400 core


in vec2 toFragShaderTexCoord;
in vec4 toFragShaderColor;
in vec3 toFragShaderNormal;
in vec3 toFragShaderPos; 
in mat4 toFragShaderModel;
in mat4 toFragShaderRotation;

out vec4 outputColor;

uniform sampler2D sampler;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

const vec3 AMBIENT_VALUE = vec3(0.1F);

void main()
{
  vec4 position              = toFragShaderModel * toFragShaderRotation * vec4(toFragShaderPos, 1.0F);
  vec4 rotatedNormal         = toFragShaderRotation * vec4(toFragShaderNormal, 1.0F);
  vec4 vectorToLightPosition = normalize(vec4(lightPosition, 1.0F) - position);
  
  float diffuseValue         = max(dot(vectorToLightPosition, rotatedNormal), 0.0F);
  
  //calculate attenuation
  float distance = length(lightPosition - vec3(position));

  const float k1 = 0;
  const float k2 = 0;
  const float k3 = 1;
  const float diffuseIntensity = 10.0F;

  float attenuation = 1 / (k1 + k2 * distance + k3 * distance * distance);
  
  //calculate specular
  vec3 vReflectionVector    = normalize(reflect(vec3(-vectorToLightPosition), vec3(rotatedNormal)));
  vec3 vEyeVectorWorld      = normalize(cameraPosition - vec3(position));
  float specularLight      = clamp(dot(vReflectionVector, vEyeVectorWorld), 0.0F, 1.0F);
  specularLight = pow(specularLight, 8);

  outputColor = texture(sampler, toFragShaderTexCoord) * (AMBIENT_VALUE + vec4(diffuseValue * diffuseIntensity * attenuation) + vec4(specularLight * attenuation)); 
  //outputColor = vec4(1.0F, 1.0F, 1.0F, 1.0F);
}
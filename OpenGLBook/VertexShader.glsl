#version 400 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 normal;

out vec2 toFragShaderTexCoord;
out vec4 toFragShaderColor;
out float toFragShaderDefuseValue;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPosition;

void main()
{
  vec4 position              = model * vec4(pos, 1.0F);
  vec4 vectorToLightPosition = normalize(vec4(lightPosition, 1.0F) - position);
  float diffuseValue         = max(dot(vectorToLightPosition, vec4(normal, 1.0F)), 0.0F);

  gl_Position = projection * view * model * vec4(pos, 1.0F);
  
  toFragShaderColor    = color;
  toFragShaderTexCoord = texCoord;
  toFragShaderDefuseValue = diffuseValue;
}
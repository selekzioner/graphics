#version 330

in vec3 posAttr;
in vec3 normAttr;
in vec2 texAttr;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;

out vec3 normal;
out vec3 lightDir;
out vec2 texCoord;
out vec3 fragPos;

void main()
{
  normal = normalize(mat3(transpose(inverse(model))) * normAttr);

  fragPos = vec3(model * vec4(posAttr, 1.0f));
  lightDir = normalize(lightPos);

  gl_Position = projection * view * model * vec4(posAttr, 1.0f);
  texCoord = texAttr;
}

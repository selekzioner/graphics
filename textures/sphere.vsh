#version 330

in vec3 posAttr;
in vec3 normAttr;
in vec2 texAttr;
in vec3 tangent;
in vec3 bitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;

out vec3 lightDir;
out vec2 texCoord;
out vec3 fragPos;
out vec3 n;
out vec3 t;
out vec3 b;


void main()
{
  mat3 normalMatrix = mat3(transpose(inverse(model)));
  n = normalize(normalMatrix * normAttr);
  t = normalize(cross(n, vec3(1.f, 1.f, 1.f)));
  b = normalize(cross(n, t));

  fragPos = vec3(model * vec4(posAttr, 1.0f));
  lightDir = normalize(lightPos);

  gl_Position = projection * view * model * vec4(posAttr, 1.0f);
  texCoord = texAttr;
}

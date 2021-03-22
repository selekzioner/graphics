#version 330

uniform sampler2D texture;
uniform sampler2D normalMap;

uniform vec4 ambCol;
uniform vec4 diffCol;
uniform vec4 specCol;

uniform vec3 viewPos;

in vec2 texCoord;
in vec3 lightDir;
in vec3 fragPos;

in vec3 n;
in vec3 t;
in vec3 b;


vec3 normal()
{
  vec3 normal = texture2D(normalMap, texCoord).rgb;
  normal = normalize(normal * 2.f - 1.f);
  vec3 result = normalize(normal.x * t + normal.y * b + normal.z * n);
  return result;
}


vec4 lightning()
{
  vec3 norm = normal();

  /* ambient */
  float ka = 0.3f;
  vec4 ambient = ka * ambCol;

  /* diffuse */
  float diff = max(dot(norm, lightDir), 0.f);
  vec4 diffuse = diff * diffCol;

  /* specular */
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 10);
  vec4 specular =  spec * specCol;

  return ambient + diffuse + specular;
}


void main(void)
{
  gl_FragColor = lightning() * texture2D(texture, texCoord);
}

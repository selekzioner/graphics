#version 330

uniform sampler2D texture;

uniform vec4 ambCol;
uniform vec4 diffCol;
uniform vec4 specCol;

uniform vec3 viewPos;

in vec2 texCoord;
in vec3 normal;
in vec3 lightDir;
in vec3 fragPos;


vec4 lightning()
{
  /* ambient */
  float ka = 0.3f;
  vec4 ambient = ka * ambCol;

  /* diffuse */
  float diff = max(dot(normal, lightDir), 0.f);
  vec4 diffuse = diff * diffCol;

  /* specular */
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, normal);

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 4);
  vec4 specular =  spec * specCol;

  return ambient + diffuse + specular;
}


void main(void)
{
  gl_FragColor = lightning() * texture2D(texture, texCoord);
}

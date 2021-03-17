#version 330

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 objectColor;
uniform vec4 ambientColor;
uniform vec4 directedColor;
uniform vec4 pointColor;
uniform vec4 projectorColor;

uniform vec3 viewPos;
uniform vec3 directedPos;
uniform vec3 pointPos;
uniform vec3 projectorPos;

in vec3 posAttr;
in vec3 normAttr;

uniform float ka;
uniform float kd;
uniform float ks;

uniform int isAmbient;
uniform int isPoint;
uniform int isDirected;
uniform int isProjector;

out vec4 col;


vec4 ambient()
{
    vec4 ambient = ka * ambientColor;
    return ambient;
}


vec4 directed(vec3 fragPos, vec3 normal)
{
    /* diffuse */
    vec3 lightDir = normalize(directedPos);
    float diff = kd * max(dot(normal, lightDir), 0.f);
    vec4 diffuse = diff * pointColor;

    /* specular */
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    vec4 specular = ks * spec * directedColor;

    return diffuse + specular;
}


vec4 point(vec3 fragPos, vec3 normal)
{
    /* diffuse */
    vec3 lightDir = normalize(pointPos - fragPos);
    float diff = kd * max(dot(normal, lightDir), 0.f);
    vec4 diffuse = diff * pointColor;

    /* specular */
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    vec4 specular = ks * spec * pointColor;

    float lightDistance = abs((pointPos.x - fragPos.x)*(pointPos.x - fragPos.x) +
                              (pointPos.y - fragPos.y)*(pointPos.y - fragPos.y) +
                              (pointPos.z - fragPos.z)*(pointPos.z - fragPos.z)) / 50;
    return (diffuse + specular) / sqrt(lightDistance);
}


vec4 projector(vec3 fragPos)
{
    vec3 normal = normalize(mat3(transpose(inverse(view * model))) * normAttr);
    /* diffuse */
    vec3 lightDir = normalize(projectorPos - fragPos);
    float diff = kd * max(dot(normal, lightDir), 0.f);
    vec4 diffuse = diff * projectorColor;

    /* specular */
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.f), 10);
    vec4 specular = ks * spec * projectorColor;

    float outerCutOff = 3, cutOff = 2;

    vec3 projDir;
    projDir.x = 0.f; projDir.y = 1.f; projDir.z = 0.f;
    float theta = dot(lightDir, normalize(-projDir));
    float intensity = clamp((theta - outerCutOff) / (cutOff - outerCutOff), 0.f, 10000000.f);
    diffuse  *= intensity;
    specular *= intensity;

    float lightDistance = abs((projectorPos.x - fragPos.x)*(projectorPos.x - fragPos.x) +
                              (projectorPos.y - fragPos.y)*(projectorPos.y - fragPos.y) +
                              (projectorPos.z - fragPos.z)*(projectorPos.z - fragPos.z))/2;

    return (diffuse + specular) / sqrt(lightDistance);
}


void main()
{
    vec3 fragPos = vec3(model * vec4(posAttr, 1.0f));
    vec3 normal = normalize(mat3(transpose(inverse(model))) * normAttr);

    col = (isAmbient * ambient() + isPoint * point(fragPos, normal)
           + isDirected * directed(fragPos, normal) + isProjector * projector(fragPos)) * objectColor;
    gl_Position = projection * view * model * vec4(posAttr, 1.0f);
}

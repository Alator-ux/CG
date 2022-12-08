#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TPos;

struct PointLight {
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 atten;
};

uniform PointLight pLight;
struct ObjMaterial {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emission;
    float shininess;
};
uniform ObjMaterial material;

uniform sampler2D text;

uniform vec3 viewPos;

out vec4 outColor;

void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightDir = normalize(pLight.pos - FragPos);
    vec3 lightReflDir = reflect(-lightDir, Normal);

    float NdotL = max(dot(Normal, lightDir), 0);
    float RdotV = max(dot(lightReflDir, viewDir), 0);

    vec3 spec = pow(RdotV, material.shininess) * pLight.specular * material.specular;
    vec3 diff = NdotL * material.diffuse * pLight.diffuse;

    vec3 res = spec + diff + pLight.ambient * material.ambient + material.emission;
    res *= vec3(texture(text, TPos));
    outColor = vec4(min(res, 1.0f), 1.0f);
}
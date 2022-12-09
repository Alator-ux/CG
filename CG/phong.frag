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

struct DirectionLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct FlashLight {
    vec3 pos;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cutOff;
    vec3 atten;
};

uniform PointLight pLight;
uniform DirectionLight dirLight;
uniform FlashLight flashLight;

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

    // Point light
    vec3 lightDir = normalize(pLight.pos - FragPos);
    vec3 lightReflDir = reflect(-lightDir, Normal);

    float NdotL = max(dot(Normal, lightDir), 0);
    float RdotV = max(dot(lightReflDir, viewDir), 0);

    vec3 spec = pow(RdotV, material.shininess) * pLight.specular * material.specular;
    vec3 diff = NdotL * material.diffuse * pLight.diffuse;

    vec3 lc1 = spec + diff;
    // -------------------

    // Direction light
    lightDir = dirLight.direction;
    lightReflDir = reflect(-lightDir, Normal);

    NdotL = max(dot(Normal, lightDir), 0);
    RdotV = max(dot(lightReflDir, viewDir), 0);

    spec = pow(RdotV, material.shininess) * dirLight.specular * material.specular;
    diff = NdotL * material.diffuse * dirLight.diffuse;

    vec3 lc2 = spec + diff;
    // -------------------

    // Flash light
    lightDir = normalize(flashLight.pos - FragPos);
    float theta = dot(lightDir, -normalize(flashLight.direction));
    vec3 lc3 = vec3(0.0f);
    //float cutOff = 1;
    if(theta > cos(radians(flashLight.cutOff))) {
        lightReflDir = reflect(-lightDir, Normal);

        NdotL = max(dot(Normal, lightDir), 0);
        RdotV = max(dot(lightReflDir, viewDir), 0);

        spec = pow(RdotV, material.shininess) * flashLight.specular * material.specular;
        diff = NdotL * material.diffuse * flashLight.diffuse;
        lc3 = spec + diff;
    }
    // -------------------


    vec3 res = lc3;
    res += dirLight.ambient * material.ambient + material.emission;
    res *= vec3(texture(text, TPos));

    outColor = vec4(min(res, 1.0f), 1.0f);
}
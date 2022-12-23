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
    float intensity = dot(lightDir, Normal);
    vec3 coef;
    if (intensity > 0.95)      coef = vec3(1.0);
    else if (intensity > 0.75) coef = vec3(0.8);
    else if (intensity > 0.50) coef = vec3(0.6);
    else if (intensity > 0.25) coef = vec3(0.4);
    else                       coef = vec3(0.2);

    vec3 lc1 = coef;
    // -------------------

    // Direction light
    lightDir = -dirLight.direction;
    intensity = dot(lightDir, Normal);
    if (intensity > 0.95)      coef = vec3(1.0);
    else if (intensity > 0.75) coef = vec3(0.8);
    else if (intensity > 0.50) coef = vec3(0.6);
    else if (intensity > 0.25) coef = vec3(0.4);
    else                       coef = vec3(0.2);

    vec3 lc2 = coef;
    // -------------------

    // Flash light
    lightDir = normalize(flashLight.pos - FragPos);
    float theta = dot(lightDir, -normalize(flashLight.direction));
    coef = vec3(0.0f);
    if(theta > cos(radians(flashLight.cutOff))){
        intensity = dot(lightDir, Normal);
        if (intensity > 0.95)      coef = vec3(1.0);
        else if (intensity > 0.75) coef = vec3(0.8);
        else if (intensity > 0.50) coef = vec3(0.6);
        else if (intensity > 0.25) coef = vec3(0.4);
        else                       coef = vec3(0.2);
    }

    vec3 lc3 = coef;
    // -------------------


    vec3 res = lc1;
    res += pLight.ambient * material.ambient + material.emission;
    res *= vec3(texture(text, TPos));

    outColor = vec4(min(res, 1.0f), 1.0f);
}
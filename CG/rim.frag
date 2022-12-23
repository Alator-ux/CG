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
    float specPower = 8.0;
    float rimPower  = 1.0;
    float bias      = 0.0;
    vec3 diffColor = vec3 ( 0.5, 0.0, 0.0);
    vec3 specColor = vec3 ( 0.7, 0.7, 0.0);
    vec3 viewDirt = normalize(viewDir);
    float rim = pow (1.0 - max(dot(Normal, viewDirt), 0.0), rimPower);
    vec3 spec = vec3(0.0);
    // Point light
    vec3 lightDir = normalize(pLight.pos - FragPos);
    
    vec3 diff = material.diffuse * pLight.diffuse;
    diff *= max(dot(lightDir, Normal), 0.0);
    vec3 lc1 = vec3(0.0);
    lc1 *= vec3(texture(text, TPos));
    lc1 += vec3(smoothstep(0.8, 1.0, rim) * vec3(0.5, 0.0, 0.2);
    // -------------------

    // Direction light
    lightDir = dirLight.direction;
    
    diff = diffColor * pLight.diffuse;
    diff *= max(dot(lightDir, Normal), 0.0);
    spec = specColor * pLight.specular;
    spec *= pow(max(dot(Normal, lightDir), 0.0), specPower);
    vec3 lc2 = diff + spec;
    lc2 *= vec3(texture(text, TPos));
    lc2 += rim * vec3(0.5, 0.0, 0.2);
    // -------------------

    // Flash light
    lightDir = normalize(flashLight.pos - FragPos);

    float theta = dot(lightDir, -normalize(flashLight.direction));
    vec3 lc3 = vec3(0.0f);
    if(theta > cos(radians(flashLight.cutOff))){
        diff = diffColor * pLight.diffuse;
        diff *= max(dot(lightDir, Normal), 0.0);
        spec = specColor * pLight.specular;
        spec *= pow(max(dot(Normal, lightDir), 0.0), specPower);
        lc3 = diff + spec;
        lc3 *= vec3(texture(text, TPos));
        lc3 += rim * vec3(0.5, 0.0, 0.2);
    }
    // -------------------


    vec3 res = lc1;
    res += dirLight.ambient * material.ambient + material.emission;
    res *= vec3(texture(text, TPos));

    outColor = vec4(min(res, 1.0f), 1.0f);
}
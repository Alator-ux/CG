#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTPos;

out vec3 FragPos;
out vec3 Normal;
out vec2 TPos;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform mat3 NModel;

struct FlashLight {
    vec3 pos;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cutOff;
    vec3 atten;
};

uniform FlashLight fLight1;
uniform FlashLight fLight2;

flat out FlashLight flashLight1;
flat out FlashLight flashLight2;

void main() {
    FragPos = vec3(Model * vec4(vPos, 1.0));
    gl_Position = Projection * View * vec4(FragPos, 1.0);
    Normal = mat3(transpose(inverse(Model))) * vNormal;
	TPos = vTPos;
    flashLight1 = fLight1;
    //flashLight1.pos = normalize(vec3(Model * vec4(flashLight1.pos, 1.f)));
    //flashLight1.direction = normalize(vec3(Model * vec4(flashLight1.direction, 1.f)));
    flashLight2 = fLight2;
    //flashLight2.pos = normalize(vec3(Model * vec4(flashLight2.pos, 1.f)));
    //flashLight2.direction = normalize(vec3(Model * vec4(flashLight2.direction, 1.f)));
}
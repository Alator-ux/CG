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

void main() {
    gl_Position = Projection * View * Model * vec4(vPos, 1.0);
    vec4 t = Model * vec4(vPos, 1.0);
    t /= t.w;
    FragPos = vec3(t);
    Normal = transpose(inverse(mat3(Model))) * vNormal;
	TPos = vTPos;
}
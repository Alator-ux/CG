#version 330 core
in vec3 vPos;
in vec3 vColor;

uniform mat4 View;
uniform mat4 Projection;

out vec3 Color;

void main() {
	gl_Position = Projection * View * vec4(vPos, 1.0);
    Color = vColor;
}
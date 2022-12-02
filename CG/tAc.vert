#version 330 core
in vec3 vPos;
in vec3 vColor;
in vec2 vTex;

uniform mat4 View;
uniform mat4 Projection;

out vec3 Color;
out vec2 Tex;

void main() {
	gl_Position = Projection * View * vec4(vPos, 1.0);
    Color = vColor;
    Tex = vTex;
}
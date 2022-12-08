#version 330 core
in vec2 TPos;
uniform sampler2D text;

layout (location = 0) out vec4 outColor;

void main() {
	outColor = texture(text, TPos);
}
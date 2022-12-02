#version 330 core
in vec2 Tex;

uniform sampler2D text1;
uniform sampler2D text2;
uniform float mixCoef;
out vec4 outColor;

void main() {
	vec4 color = mix(texture(text1, Tex), texture(text2, Tex), mixCoef);
    outColor = color;
}
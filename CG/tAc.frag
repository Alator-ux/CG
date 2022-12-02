#version 330 core
in vec3 Color;
in vec2 Tex;

uniform sampler2D textur;

out vec4 outColor;

void main() {
	vec4 color = mix(texture(textur, Tex), vec4(Color,1.0), 0.5);
    outColor = color;
}
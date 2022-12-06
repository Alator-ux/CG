#version 330 core
in vec2 TCoord;
flat in int index;
uniform sampler2D text[10];

void main() {
	gl_FragColor = texture(text[index], TCoord);
}
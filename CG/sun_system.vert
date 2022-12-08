#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTPos;

out vec2 TPos;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model[2];

uniform int offset;
uniform vec4 shifts[6];

mat4 yRotMat(in float angle) {
    float cs = cos(angle);
    float sn = sin(angle);
	return mat4(cs, 0.0, sn, 0.0,
			 	0.0, 1.0, 0.0, 0.0,
				-sn, 0.0, cs, 0.0,
				0.0, 0.0, 0.0, 1.0);
}

void main() {
    int ind = offset + gl_InstanceID;
	float shift = shifts[ind].x;
    float scale = shifts[ind].y;
	float rot_axis = shifts[ind].z;
	float rot_sun = shifts[ind].w;
	vec4 pos = yRotMat(rot_axis) * vec4(vPos * scale, 1.0);
    pos = yRotMat(rot_sun) * (pos + vec4(shift, 0.0, 0.0, 0.0));
    mat4 model = Model[offset];
    gl_Position = Projection * View * model * pos;
	TPos = vTPos;
}
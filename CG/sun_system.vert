#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTPos;

out vec2 TPos;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

uniform int offset;
uniform vec4 mutations[6];

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
	vec4 cur = mutations[ind];
	float shift = cur.x;
    float scale = cur.y;
	float rot_axis = cur.z;
	float rot_orbit = cur.w;
	vec4 pos = yRotMat(rot_axis) * vec4(vPos * scale, 1.0);
    pos = yRotMat(rot_orbit) * (pos + vec4(shift, 0.0, 0.0, 0.0));
    gl_Position = Projection * View * Model * pos;
	TPos = vTPos;
}
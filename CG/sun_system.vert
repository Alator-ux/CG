#version 330 core
in vec3 vPos;
in vec2 vTCoord;
out vec2 TCoord;
flat out int index;
uniform mat4 ProjectionViewModel;
uniform vec4 offsets[10];

mat4 yRotMat(in float angle) {
    float cs = cos(angle);
    float sn = sin(angle);
	return mat4(cs, 0.0, sn, 0.0,
			 	0.0, 1.0, 0.0, 0.0,
				-sn, 0.0, cs, 0.0,
				0.0, 0.0, 0.0, 1.0);
}

void main() {
	float offset = offsets[gl_InstanceID].x;
    float scale = offsets[gl_InstanceID].y;
	float rot_axis = offsets[gl_InstanceID].z;
	float rot_sun = offsets[gl_InstanceID].w;
	vec4 pos = yRotMat(rot_axis) * vec4(vPos * scale, 1.0);
    pos = yRotMat(rot_sun) * (pos + vec4(offset, 0.0, 0.0, 0.0));
    gl_Position = ProjectionViewModel * pos;
	Tex = vTex;
	index = gl_InstanceID;
}
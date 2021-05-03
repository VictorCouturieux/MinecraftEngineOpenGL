#version 400

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform mat4 nmat;
uniform float elapsed;

layout(location=0) in vec3 vs_position_in;
layout(location=1) in vec3 vs_normal_in;
layout(location=2) in vec2 vs_uv_in;
layout(location=3) in float vs_type_in;

//Variables en sortie
out vec3 normal;
out vec4 color;
out vec3 wPos;
flat out int type;

#define CUBE_HERBE 0.0
#define CUBE_TERRE 1.0
#define CUBE_EAU 4.0

float vagues(vec3 pos){
	float h = sin(pos.x / 30 + elapsed) * 10;
	h += sin((0.3*pos.x+0.7*pos.y) / 10 + elapsed * 1.5) * 1.2;
	h += sin((0.6*pos.x+0.4*pos.y) / 2 + elapsed * 1.7) * 0.2f;
	return h;
}

void main()
{
	normal = (nmat * vec4(vs_normal_in,1)).xyz;
	

	vec4 pos_w = m * vec4(vs_position_in, 1.0);
	wPos = pos_w.xyz;

	type = int(vs_type_in);
	if(vs_type_in == CUBE_HERBE)
		color = vec4(0,1,0,1);
	if(vs_type_in == CUBE_TERRE)
		color = vec4(1,0.6,0.2,1);
	if(vs_type_in == CUBE_EAU){
		color = vec4(0,0,1,1);
		pos_w.z = pos_w.z - 4 + vagues(pos_w.xyz);
	}
				
	gl_Position = p * v * pos_w;

}
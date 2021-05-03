#version 400

//uniform mat4 mvp;
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
out vec4 posWorld;
flat out int type;
out vec2 uv;

#define CUBE_HERBE 0.0
#define CUBE_TERRE 1.0
#define CUBE_EAU 4.0

float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float water(vec3 pos){
	return -1 + sin(pos.x/2 + elapsed) *0.2;
	//return -5 + 
	//sin(pos.x/40 + elapsed) *2 +
	//sin((0.3 * pos.x + 0.6 * pos.y )/20 + elapsed) *1 +
	//sin((0.6 * pos.x + 0.5 * pos.y )/2 + 2*elapsed) * 0.3+
	//sin((0.6 * pos.x + 0.2 * sin(pos.y) )* sin( (pos.x + pos.y)/40) + 2*elapsed) * 0.02;
}

void main()
{
	// Transforming The Vertex
	

	vec4 posO = vec4(vs_position_in,1.0);
	vec4 posW = m * posO;
	posWorld = posW;
	type = int(vs_type_in);

	//Couleur
	if(vs_type_in == CUBE_HERBE)
		color = vec4(0,1,0,1);
	if(vs_type_in == CUBE_TERRE)
		color = vec4(1,0.7,0.2,1);
	if(vs_type_in == CUBE_EAU)
		color = vec4(0,0,1,0.8f);

	//Position
	if(vs_type_in == CUBE_EAU)
	{
		posW.z += water(posW.xyz);
	}
	

	uv = vs_uv_in;

	//Espace view
	vec4 posV = v * posW;

	normal = vs_normal_in;
	//normal = (nmat * vec4(vs_normal_in,1)).xyz;
	gl_Position = p * posV;
	
	//vec4 vecIn = vec4(vs_position_in,1.0);
	//gl_Position = mvp * vecIn;
		
	//normal = (nmat * vec4(vs_normal_in,1.0)).xyz; 

	//uv = vs_uv_in;

	////Couleur par défaut violet
	//color = vec4(1.0,1.0,0.0,1.0);
	
	//Couleur fonction du type
	//if(vs_type_in == CUBE_HERBE)
	//	color = vec4(0,1,0,1);
	//if(vs_type_in == CUBE_TERRE)
	//	color = vec4(0.2,0.1,0,1);
	//if(vs_type_in == CUBE_EAU)
	//	color = vec4(0.0,0.0,1.0,0.7);
	

}
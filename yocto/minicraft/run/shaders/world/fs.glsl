#version 400

//Variables en entree
in vec3 normal;
in vec4 color;
in vec4 posWorld;
flat in int type;
//in vec2 uv;

out vec4 p_color;
//out vec4 color_out;

uniform vec3 light_pos;
uniform vec3 cam_pos; //Position de la camera
uniform float elapsed;
uniform vec4 sun_color; //Couleur du soleil

#define CUBE_HERBE 0.0
#define CUBE_TERRE 1.0
#define CUBE_EAU 4.0

//Globales
//const float ambientLevel = 0.4;

float water(vec3 pos){
	return -1 + sin(pos.x/2 + elapsed) *0.2;
	//-5 + 
	//sin(pos.x/40 + elapsed) *2 +
	//sin((0.3 * pos.x + 0.6 * pos.y )/20 + elapsed) *1 +
	//sin((0.6 * pos.x + 0.5 * pos.y )/2 + 2*elapsed) * 0.3+
	//sin((0.6 * pos.x + 0.2 * sin(pos.y) )* sin( (pos.x + pos.y)/40) + 2*elapsed) * 0.02;
}

void main()
{
	vec3 Lsun = normalize(light_pos - cam_pos);
	vec3 L2 = normalize(cam_pos - posWorld.xyz);
	float L2Power = 1.0 / (length(cam_pos - posWorld.xyz)/60);
	vec3 N = normal;

	if(type == CUBE_EAU){
		vec3 P = posWorld.xyz;
		vec3 A = P + vec3(1,0,0);
		vec3 B = P + vec3(0,1,0);

		P.z += water(P); 
		A.z += water(A); 
		B.z += water(B); 

		N = normalize(cross(A-P,B-P));
	}

	float hautDansLeCiel = min(Lsun.z*0.7,0.7);
	float ambient = hautDansLeCiel;

	float diffuse = max(ambient,dot(N,Lsun));
	//diffuse += max(0,dot(N,L2)) * L2Power;

	//Speculaire
	vec3 V = normalize(cam_pos - posWorld.xyz);
	vec3 H = normalize(Lsun + V);
	float spec = pow(max(0,dot(N,H)),300) * 10 ;

	vec3 specColor = spec * sun_color.rgb;

	//color_out = vec4((color.rgb + specColor)*diffuse,color.a);
	p_color = vec4((color.rgb + specColor)*diffuse,color.a);
	//p_color.rgb = N.rgb;
	

	//////////////////////////
	//vec3 toLight = normalize(vec3(0,1,1));
	//color_out = vec4(sqrt(color.xyz * max(0,dot(toLight,normal)) * 0.97 + 0.03 * vec3(0.8,0.9,1)),color.a);
	
}
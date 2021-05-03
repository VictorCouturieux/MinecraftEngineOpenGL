#version 400

	in vec3 normal;
	in vec4 color;
	in vec3 wPos;
	flat in int type;

	out vec4 p_color;

	uniform vec3 light_pos;
	uniform vec4 bg_color;
	uniform vec3 cam_pos; //Position de la camera
	uniform vec4 sun_color; //Couleur du soleil
	uniform float elapsed;

	#define CUBE_HERBE 0.0
	#define CUBE_TERRE 1.0
	#define CUBE_EAU 4

	float vagues(vec3 pos){
		float h = sin(pos.x / 30 + elapsed) * 10;
		h += sin((0.3*pos.x+0.7*pos.y) / 10 + elapsed * 1.5) * 1.2;
		h += sin((0.6*pos.x+0.4*pos.y) / 2 + elapsed * 1.7) * 0.2f;
		return h;
	}

	void main()
	{
		vec3 L = normalize(light_pos-cam_pos);
		vec3 N = normalize(normal);
		vec3 V = normalize(cam_pos - wPos);
		vec3 H = normalize(V + L);

		if(type == CUBE_EAU){
			vec3 A = wPos;
			vec3 B = A + vec3(0.5f,0,0);	
			vec3 C = A + vec3(0,0.5f,0);
			A.z = A.z - 4 + vagues(A);
			B.z = B.z - 4 + vagues(B);	
			C.z = C.z - 4 + vagues(C);
			N = normalize(cross(normalize(B-A), normalize(C-A)));	
		}


		float d = max(0,dot(L,N));
		float s = 20 * 1 *  pow(max(0,dot(H,N)),300 * 1+1);
		
		vec3 diffuse = color.xyz * d;
		vec3 speculaire = sun_color.xyz * s	* d;
		vec3 ambiante = bg_color.xyz  *color.xyz * 1;

		
		vec3 colorFinale = diffuse + speculaire + ambiante;

		p_color = vec4(colorFinale,color.a);	
	}
#version 120

uniform sampler2D TexColor;
//uniform sampler2D TexDepth;

uniform float screen_width; //Largeur de l'écran en pixels
uniform float screen_height; //Hauteur de l'écran en pixels

float LinearizeDepth(float z)
{
	float n = 0.5; // camera z near
  	float f = 10000.0; // camera z far
  	return (2.0 * n) / (f + n - z * (f - n));
}

void main (void)
{
	vec3 pixSize = vec3(1/screen_width,1/screen_height,0);
	vec2 uv = vec2( gl_TexCoord[0] );

	vec4 color = texture2D( TexColor , uv);

	float size = 1;
	float nb = 0;
	vec3 colorSum = vec3(0,0,0);

	for(float x = -size;x <= size; x++){
		for(float y = -size;y <= size; y++){
			vec2 uv2 = uv + vec2(pixSize.x*x,pixSize.y*y);
			colorSum += texture2D( TexColor , uv2).xyz;
			nb++;
		}		
	}

	colorSum -= nb * color.xyz; 


	////float depth = LinearizeDepth(texture2D( TexDepth , uv).x);
	////color += texture2D( TexColor , uv+pixSize.xz * 3);
	////color += texture2D( TexColor , uv-pixSize.xz * 3);

	////color = vec4(depth,0,0,1);

	gl_FragColor = vec4(colorSum.rgb,1.0);
}
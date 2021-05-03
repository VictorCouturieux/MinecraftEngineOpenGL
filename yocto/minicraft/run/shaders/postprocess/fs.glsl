#version 120

in vec2 uv;

uniform sampler2D TexColor;
uniform sampler2D TexDepth;

uniform float screen_width; //Largeur de l'écran en pixels
uniform float screen_height; //Hauteur de l'écran en pixels
//uniform vec2 near_far;

out vec4 color_out;

float LinearizeDepth(float z)
{
	float n = 0.5; // camera z near
  	float f = 10000.0; // camera z far
  	return (2.0 * n) / (f + n - z * (f - n));

	///////////////*/
	//float n = near_far.x; // camera z near
  	//float f = near_far.y; // camera z far
  	//return (2.0 * n) / (f + n - z * (f - n));
	///////////////*/
}

void main (void)
{ 
	vec4 colorBase = texture2D( TexColor , vec2( gl_TexCoord[0] ) );	
	
	float flou = 5;
    vec2 pixSize = vec2(1.0/screen_width, 1.0/screen_height);

    vec4 colorFloue = colorBase;
	colorFloue += texture2D( TexColor , vec2( gl_TexCoord[0] ) + vec2(pixSize.x,0)*flou );	
	colorFloue += texture2D( TexColor , vec2( gl_TexCoord[0] ) + vec2(-pixSize.x,0)*flou );
	colorFloue += texture2D( TexColor , vec2( gl_TexCoord[0] ) + vec2(0,pixSize.y)*flou );	
	colorFloue += texture2D( TexColor , vec2( gl_TexCoord[0] ) + vec2(0,-pixSize.y)*flou );
	
	colorFloue /= 5;

	float depth = LinearizeDepth(texture2D( TexDepth , vec2( gl_TexCoord[0] )).r);
	float depthCenter = LinearizeDepth(texture2D( TexDepth , vec2(0.5,0.5)).r);
	
	vec4 color = mix(colorBase,colorFloue,0);

	
	//float sobel = 2;
	//vec4 colorSobel = 4*colorBase;
	//colorSobel -= texture2D( TexColor , vec2( gl_TexCoord[0] ) + vec2(pixSize.x,0)*sobel );	
	//colorSobel -= texture2D( TexColor , vec2( gl_TexCoord[0] ) + vec2(-pixSize.x,0)*sobel );
	//colorSobel -= texture2D( TexColor , vec2( gl_TexCoord[0] ) + vec2(0,pixSize.y)*sobel );	
	//colorSobel -= texture2D( TexColor , vec2( gl_TexCoord[0] ) + vec2(0,-pixSize.y)*sobel );
	//color = colorSobel; 
	

	gl_FragColor = vec4(sqrt(color.rgb),1.0);
	
	/////////////////////////
	//////////////////////////
	//float xstep = 1.0/screen_width;
	//float ystep = 1.0/screen_height;
	//float ratio = screen_width / screen_height;

	//vec4 color = texture2D( TexColor , uv );
	//float depth = texture2D( TexDepth , uv ).r;	
	
	////Permet de scaler la profondeur
	//depth = LinearizeDepth(depth);

    ////Gamma correction
    //color.r = pow(color.r,1.0/2.2);
    //color.g = pow(color.g,1.0/2.2);
    //color.b = pow(color.b,1.0/2.2);

	//color_out = vec4(color.rgb,1.0);
}
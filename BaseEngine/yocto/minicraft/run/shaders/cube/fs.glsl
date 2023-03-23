#version 400

in vec2 UV;

uniform sampler2D colorTex1;

//in vec4 color_out_vs;

out vec4 color_out;

void main()
{
	//color_out = vec4(sqrt(color_out_vs.rgb),1);

	color_out = texture( colorTex1, UV).rgba;
	//color_out = vec4(0,0,1,1);
}
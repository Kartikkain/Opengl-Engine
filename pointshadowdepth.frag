#version 330 core

in vec4 FragPos;

uniform vec3 lightpos;
uniform float far_plane;

void main()
{
	float lightdistance = length(FragPos.xyz - lightpos);

	lightdistance = lightdistance / far_plane;

	gl_FragDepth = lightdistance;

}
#version 330 core

out vec4 FragColor;

in vec2 texcoords;

uniform sampler2D texture1;

void main()
{
	vec4 TexColor = texture(texture1,texcoords);
	//if(TexColor.a < 0.1) discard;

	FragColor = texture(texture1,texcoords);
}
#version 330 core

layout ( location = 0 ) out vec4 FragColor;
layout ( location = 1 ) out vec4 BrightColor;

void main()
{
	FragColor = vec4(1.0f);

	float brightness = dot( FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));

	BrightColor = (brightness > 1.0) ? vec4( FragColor.rgb, 1.0) : vec4(0.0,0.0,0.0,1.0);
}
#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform samplerCube skybox;
uniform vec3 campos;

void main()
{
	vec3 I = normalize(FragPos - campos);
	vec3 R = reflect(I, normalize(Normal));
	FragColor = vec4 ( texture(skybox,R).rgb,1.0f);
}
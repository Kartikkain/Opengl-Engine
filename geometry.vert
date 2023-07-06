#version 330 core

layout ( location = 0 ) in vec3 aPos;
layout ( location = 1 ) in vec2 aTexCoor;
layout ( location = 2 ) in vec3 aNormal;


out vec2 Texcoords;
out vec3 geoNormal;
out vec3 geoFragPos;
uniform mat4 model;
uniform mat4 view;
//uniform mat4 projection;

void main()
{
	gl_Position = view * model * vec4(aPos,1.0f);
	geoFragPos = vec3( model * vec4(aPos,1.0f));
	Texcoords = aTexCoor;
	geoNormal  = mat3(transpose(inverse(model))) * aNormal;
}
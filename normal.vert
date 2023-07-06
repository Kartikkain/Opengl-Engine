#version 330 core

layout ( location = 0 ) in vec3 aPos;
layout ( location = 1 ) in vec2 aTexCoor;
layout ( location = 2 ) in vec3 aNormal;


out vec2 Texcoords;
out vec3 geoNormal;
out vec3 geoFragPos;

uniform mat4 model;
uniform mat4 view;

void main()
{
	mat3 normalmatrix = mat3(transpose(inverse(view * model )));
	geoNormal  = vec3(vec4( normalmatrix * aNormal,0.0 ));
	gl_Position = view * model * vec4(aPos,1.0f);
}
#version 330 core

layout ( location = 0 ) in vec3 aPos;
layout ( location = 1 ) in vec2 aTexCoor;
layout ( location = 3 ) in mat4 aInstanceMatrix;

out vec2 texcoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	texcoords = aTexCoor;
    gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 1.0f); 
}
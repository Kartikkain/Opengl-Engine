#version 330 core

layout ( location = 0 ) in vec3 aPos;
layout ( location = 1 ) in vec2 aTexCoor;
layout ( location = 2 ) in vec3 aNormal;
layout ( location = 3 ) in vec3 aTangent;
layout ( location = 4 ) in vec3 aBitangent;

out vec2 texcoords;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;
uniform vec3 lightpos;
uniform vec3 campos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos,1.0f);
	FragPos = vec3( model * vec4(aPos,1.0f));
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos,1.0);
	texcoords = aTexCoor;
	Normal  = mat3(transpose(inverse(model))) * aNormal;


	//mat3 normalMatrix = transpose(inverse(mat3(model)));
	//vec3 T = normalize(normalMatrix * aTangent);
	//T = normalize( T - dot(T,Normal) * Normal);
	//vec3 B = cross(Normal, T);

	vec3 T = normalize(mat3(model) * aTangent);
	vec3 B = normalize(mat3(model) * aBitangent);
	vec3 N = normalize(mat3(model) * aNormal);
	mat3 TBN = transpose(mat3(T,B,Normal));

	TangentLightPos = TBN * lightpos;
	TangentViewPos = TBN * campos;
	TangentFragPos = TBN * FragPos;

}
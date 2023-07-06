#version 330 core

layout ( triangles ) in;
layout ( triangle_strip ,max_vertices = 18) out;

out vec4 FragPos;

uniform mat4 shadowMatrices[6];

void main()
{
	
	for(int faces = 0; faces < 6 ; ++faces)
	{
		gl_Layer = faces;

		for(int i = 0 ;i < 3 ;++i)
		{
			FragPos = gl_in[i].gl_Position;
			gl_Position = shadowMatrices[faces] * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
	
}
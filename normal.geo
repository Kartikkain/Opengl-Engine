#version 330 core

layout ( triangles ) in;
layout ( line_strip, max_vertices = 6 ) out;

in vec2 Texcoords[];
in vec3 geoNormal[];
in vec3 geoFragPos[];

out vec2 texcoords;
out vec3 Normal;
out vec3 FragPos;

float Magnitude = 0.4f;
uniform mat4 projection;

void GenrateLines(int index)
{
	gl_Position = projection * gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = projection * ( gl_in[index].gl_Position + vec4(geoNormal[index],0.0) * Magnitude);
	EmitVertex();
	EndPrimitive();

}

void main()
{	
	GenrateLines(0);
	GenrateLines(1);
	GenrateLines(2);
}
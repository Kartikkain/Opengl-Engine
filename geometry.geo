#version 330 core

layout ( triangles ) in;
layout ( triangle_strip, max_vertices = 3 ) out;


in vec2 Texcoords[];
in vec3 geoNormal[];
in vec3 geoFragPos[];

out vec2 texcoords;
out vec3 Normal;
out vec3 FragPos;


uniform float time;
uniform mat4 projection;

vec3 GetNormals()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);

    return normalize(cross(a,b));
}

vec4 Explode(vec4 position, vec3 normals)
{
   // vec3 direction = normals * vec3( ( sin(time) + 1.0f ) / 2.0f );
    return position + vec4(normals,0.0);
}

void main()
{
   
   

    gl_Position = projection * Explode(gl_in[0].gl_Position,geoNormal[0]);
    texcoords = Texcoords[0];
    FragPos = geoFragPos[0];
    Normal = geoNormal[0];
    EmitVertex();

    gl_Position = projection * Explode(gl_in[1].gl_Position,geoNormal[1]);
    texcoords = Texcoords[1];
    FragPos = geoFragPos[1];
    Normal = geoNormal[1];
    EmitVertex();

    gl_Position = projection * Explode(gl_in[2].gl_Position,geoNormal[2]);
    texcoords = Texcoords[2];
    FragPos = geoFragPos[2];
    Normal = geoNormal[2];
    EmitVertex();

    EndPrimitive();
}
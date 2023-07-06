#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> Verticies, std::vector<unsigned int> Indicies, std::vector<Texture> Textures)
{
	verticies = Verticies;
	indicies = Indicies;
	textures = Textures;
	SetupMesh();
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseN = 1;
	unsigned int specularN = 1;

	for (unsigned int i = 0;i < textures.size();i++)
	{
		textures[i].Active(GL_TEXTURE0 + i);
		std::string number="";
		std::string name = textures[i].textype;
		if (name == "diffuse") number = std::to_string(diffuseN++);
		else if (name == "specular") number = std::to_string(specularN++);
		std::string texturename = "material." + name + number;
		unsigned int texloc = glGetUniformLocation(shader.ID, texturename.c_str());
		glUniform1i(texloc, i);
		textures[i].Bind();
	}

	glActiveTexture(GL_TEXTURE0);
	vao.Bind();
	glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
	vao.Unbind();
}

void Mesh::SetupMesh()
{
	vao.Bind();
	vbo.Bind();
	vbo.BufferData(verticies.size() * sizeof(Vertex), &verticies[0]);
	ebo.Bind();
	ebo.Bufferdata(indicies.size() * sizeof(unsigned int), &indicies[0]);
	vao.vertexattrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	vao.vertexattrib(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texturecoords));
	vao.vertexattrib(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	vao.vertexattrib(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	vao.vertexattrib(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	vbo.Unbind();
	vao.Unbind();
}
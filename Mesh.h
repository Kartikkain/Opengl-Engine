#ifndef MESH_H
#define MESH_H
#include<iostream>
#include<vector>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"

struct Vertex {
	glm::vec3 position;
	glm::vec2 texturecoords;
	glm::vec3 normal;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};


class Mesh {
public:
	VAO vao;
	std::vector<Vertex> verticies;
	std::vector<unsigned int> indicies;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> Verticies, std::vector<unsigned int> Indicies, std::vector<Texture> Textures);
	void Draw(Shader& shader);

private:
	VBO vbo;
	EBO ebo;
	void SetupMesh();
};
#endif // !MESH_H


#include "Model.h"

void Model::Draw(Shader& shader)
{
	for (unsigned int i = 0;i < meshes.size();i++) meshes[i].Draw(shader);
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP" << import.GetErrorString() << std::endl;
		return;
	}

	// get relative path from solution file.
	directory = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0;i < node->mNumMeshes;i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0;i < node->mNumChildren;i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indicies;
	std::vector<Texture> texture;

	for (unsigned int i = 0;i < mesh->mNumVertices;i++)
	{
		Vertex vertex;

		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;

		vertex.position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;

		vertex.normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;

			vertex.texturecoords = vec;

			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;

			vertex.Tangent = vector;

			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;

			vertex.Bitangent = vector;
		}
		else vertex.texturecoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0;i < mesh->mNumFaces;i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0;j < face.mNumIndices;j++)
		{
			indicies.push_back(face.mIndices[j]);	
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
		texture.insert(texture.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> SpecularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
		texture.insert(texture.end(), SpecularMaps.begin(), SpecularMaps.end());
		std::vector<Texture> NormalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "normalMap");
		texture.insert(texture.end(), NormalMaps.begin(), NormalMaps.end());
	}

	return Mesh(vertices, indicies, texture);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string TypeName)
{
	std::vector<Texture> textures;
	for (int i = 0;i < mat->GetTextureCount(type);i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0;j < texture_loaded.size();j++)
		{
			if (std::strcmp(texture_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(texture_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			const char* path = str.C_Str();
			std::string filename = std::string(path);
			// add the path with file name to get the texture location
			filename = directory + '/' + filename;
			Texture texture(filename.c_str(), TypeName, GL_TEXTURE_2D, GL_UNSIGNED_BYTE,*g);
			texture.path = str.C_Str();
			textures.push_back(texture);
			texture_loaded.push_back(texture);
		}
	}
	return textures;
}
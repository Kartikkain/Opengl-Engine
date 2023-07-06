#ifndef MODEL_CLASS
#define MODEL_CLASS

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include "Mesh.h"

class Model
{
public:
	std::vector<Mesh> meshes;
	std::vector<Texture> texture_loaded;
	bool* g;
	Model(const char* path ,bool& gamma)
	{
		g = &gamma;
		LoadModel(path);
	}

	void Draw(Shader& shader);
private:
	
	std::string directory;

	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string TypeName);
};

#endif // !MODEL_CLASS


#ifndef TEXTURE_H
#define TEXTURE_H
#include<glm/glm.hpp>
#include<glad/glad.h>
#include<iostream>
#include<vector>
#include<stb/stb_image.h>

class Texture {
	std::vector<std::string> faces;
	GLenum target;
public:
	unsigned int ID;
	std::string path;
	std::string textype;
	Texture(std::vector<std::string> faces,std::string Textype, GLenum Target, GLenum type, bool& gamma);
	Texture(const char* texture, std::string Textype, GLenum Target, GLenum type, bool& gamma);
	void cubetex(std::vector<std::string> face);
	void Active(GLenum textureunit);
	void Bind();
};
#endif // !TEXTURE_H


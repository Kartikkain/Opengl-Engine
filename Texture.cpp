#include "Texture.h"

Texture::Texture(std::vector<std::string> faces, std::string Textype, GLenum Target, GLenum type, bool& gamma)
{
	target = Target;
	textype = Textype;
	int width, height, colorch;
	glGenTextures(1, &ID);
	glBindTexture(Target, ID);
	glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_REPEAT);

	for (unsigned int i = 0;i < faces.size();i++)
	{

		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &colorch, 0);

		GLenum format;
		if (colorch == 1) format = GL_RED;
		else if (colorch == 3) format = GL_RGB;
		else if (colorch == 4) format = GL_RGBA;

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap tex fail to load." << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

Texture::Texture(const char* texture, std::string Textype,GLenum Target,GLenum type, bool& gamma)
{
	target = Target;
	textype = Textype;
	int width, height, colorch;
	glGenTextures(1, &ID);
	glBindTexture(Target, ID);
	
	
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(texture, &width, &height, &colorch, 0);

	GLenum internalformat;
	GLenum format;
	if (colorch == 1)
	{
		internalformat = GL_RED;
		format = GL_RED;
	}
	else if (colorch == 3)
	{
		internalformat = (gamma) ? GL_SRGB : GL_RGB;
		format = GL_RGB;
	}
	else if (colorch == 4)
	{
		internalformat = (gamma) ? GL_SRGB_ALPHA : GL_RGBA;
		format = GL_RGBA;
	}

	glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(Target, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(Target, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);

	if (data)
	{

		glTexImage2D(Target, 0, internalformat, width, height, 0, format, type, data);
		glGenerateMipmap(Target);
	}
	else std::cout << "ERROR::TEXTUER::FAILED_TO_LOAD_THE_IMAGE_DATA" << std::endl;

	
	stbi_image_free(data);
	
}

void Texture::cubetex(std::vector<std::string> face)
{
	faces = face;
}

void Texture::Active(GLenum textureunit)
{
	glActiveTexture(textureunit);
}

void Texture::Bind()
{
	glBindTexture(target, ID);
	
}
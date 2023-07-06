#include"Shader.h"

Shader::Shader(const char* VertexFile, const char* FragementFile, const char* GeometryFile)
{
	std::string VertexCode;
	std::string FragementCode;
	std::string GeometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream GShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	GShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vShaderFile.open(VertexFile);
		fShaderFile.open(FragementFile);

		std::stringstream vShaderStream, fShaderStream;
		
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		VertexCode = vShaderStream.str();
		FragementCode = fShaderStream.str();

		if (GeometryFile != nullptr)
		{
			GShaderFile.open(GeometryFile);
			std::stringstream gShaderStream;
			gShaderStream << GShaderFile.rdbuf();
			GShaderFile.close();
			GeometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	const char* vShaderCode = VertexCode.c_str();
	const char* fShaderCode = FragementCode.c_str();

	unsigned int vertex, fragement;
	int success;
	char infolog[1024];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 1024, NULL, infolog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION FAILED\n" << infolog << std::endl;
	}

	fragement = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragement, 1, &fShaderCode, NULL);
	glCompileShader(fragement);

	glGetShaderiv(fragement, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragement, 1024, NULL, infolog);
		std::cout << "ERROR::SHADER::FRAGEMENT::COMPILATION FAILED\n" << infolog << std::endl;
	}

	unsigned int geometry;

	if (GeometryFile != nullptr)
	{
		const char* gShaderCode = GeometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);

		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(geometry, 1024, NULL, infolog);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION FAILED\n" << infolog << std::endl;
		}
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragement);
	if (GeometryFile != nullptr) glAttachShader(ID, geometry);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_COMPILE_STATUS, &success);
	
	if (!success)
	{
		glGetProgramInfoLog(ID, 1024, NULL, infolog);
		std::cout << "ERROR::SHADERPROGRAME::COMPILATION FAILED\N" << infolog << std::endl;
	}

	Active();

	glDeleteShader(vertex);
	glDeleteShader(fragement);
	if (GeometryFile != nullptr) glDeleteShader(geometry);
}

void Shader::Active()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

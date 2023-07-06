#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>
#include<fstream>
#include<sstream>
#include<iostream>
#include<string>
class Shader {
public:
	unsigned int ID;
	Shader(const char* VertexFile, const char* FragementFile, const char* GeometryFile = nullptr);
	void Active();
	void Delete();
};
#endif 


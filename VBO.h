#ifndef VBO_H
#define VBO_H
#include<glm/glm.hpp>
#include<glad/glad.h>
class VBO {
public:
	unsigned int ID;

	VBO();
	void Bind();
	void BufferData(GLsizeiptr size, const void* data);
	void Unbind();
	void Delete();
		
};
#endif // !VBO_CLASS_H


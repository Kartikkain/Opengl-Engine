#ifndef EBO_H
#define EBO_H

#include<glm/glm.hpp>
#include<glad/glad.h>

class EBO {
public:
	unsigned int ID;

	EBO();
	void Bind();
	void Bufferdata(GLsizeiptr size, const void* data);
	void Unbind();
	void Delete();
};
#endif // !EBO_H


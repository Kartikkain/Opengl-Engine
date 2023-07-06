#ifndef VAO_H
#define VAO_H

#include<glm/glm.hpp>
#include<glad/glad.h>

class VAO {
public:
	unsigned int ID;
	VAO();
	void Bind();
	void vertexattrib(GLuint index, GLint size, GLenum type, GLboolean normalize, GLsizei stride, const void* offset);
	void Unbind();
	void Delete();
};
#endif // VAO_H


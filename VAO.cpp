#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::vertexattrib(GLuint index, GLint size, GLenum type, GLboolean normalize, GLsizei stride, const void* offset)
{
	glVertexAttribPointer(index, size, type, normalize, stride, offset);
	glEnableVertexAttribArray(index);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}

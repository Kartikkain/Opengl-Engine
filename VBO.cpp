#include "VBO.h"

VBO::VBO()
{
	glGenBuffers(1, &ID);
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::BufferData(GLsizeiptr size, const void* data)
{
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}
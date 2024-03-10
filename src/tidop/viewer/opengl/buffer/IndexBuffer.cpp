#include "IndexBuffer.h"

namespace tl
{

IndexBuffer::IndexBuffer(const std::vector<unsigned int> indices)
	: Buffer() {
	initBuffer(indices);
}

IndexBuffer::~IndexBuffer()
{
	unbind();
	glDeleteBuffers(1, &id);
}

void IndexBuffer::initBuffer(std::vector<unsigned int> indices)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices), &indices[0], GL_DYNAMIC_DRAW);
}

void IndexBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}
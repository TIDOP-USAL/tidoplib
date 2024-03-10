#include "VertexBuffer.h"

#include <string.h>

namespace tl
{

constexpr unsigned int stride = 7 * sizeof(float);

VertexBuffer::VertexBuffer() : Buffer(), hasIdxBuffer(false) { }

VertexBuffer::VertexBuffer(std::vector<Vertex>& vertices)
    : Buffer(), hasIdxBuffer(false)
{
    initBuffer(vertices);
}

VertexBuffer::VertexBuffer(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : Buffer(), hasIdxBuffer(true)
{
    initBuffer(vertices, indices);
}

VertexBuffer::VertexBuffer(const VertexBuffer& vertexBuffer)
{
    id = vertexBuffer.id;
    hasIdxBuffer = vertexBuffer.hasIdxBuffer;
}

VertexBuffer::VertexBuffer(VertexBuffer&& vertexBuffer) noexcept
{
    id = vertexBuffer.id;
    hasIdxBuffer = vertexBuffer.hasIdxBuffer;
}

VertexBuffer& VertexBuffer::operator=(const VertexBuffer& vertexBuffer)
{
    id = vertexBuffer.id;
    hasIdxBuffer = vertexBuffer.hasIdxBuffer;
    return *this;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& vertexBuffer) noexcept
{
    id = vertexBuffer.id;
    hasIdxBuffer = vertexBuffer.hasIdxBuffer;
    return *this;
}

VertexBuffer::~VertexBuffer()
{
    unbind();
    glDeleteBuffers(1, &id);
}

void VertexBuffer::vertexAttributes()
{
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
}

void VertexBuffer::initBuffer(std::vector<Vertex>& vertices, const std::vector<unsigned int> indices)
{
    // Load vertices
    unsigned int index = 0;
    float* glVertices = new float[vertices.size() * 7];

    for (Vertex& vertex : vertices) {

        glVertices[index] = vertex.position[0];       glVertices[index + 1] = vertex.position[1];   glVertices[index + 2] = vertex.position[2];
        glVertices[index + 3] = vertex.color[0];      glVertices[index + 4] = vertex.color[1];   glVertices[index + 5] = vertex.color[2];
        glVertices[index + 6] = vertex.color[3];

        index += 7;
    }

    // Vertex buffer
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * stride, glVertices, GL_DYNAMIC_DRAW);
    delete[] glVertices;

    // Index Buffer
    if (hasIdxBuffer) indexBuffer = IndexBuffer::New(indices);

    // Vertex Attributes
    vertexAttributes();

    // Unbind VBO
    unbind();
}


void VertexBuffer::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}
#include "VertexBuffer.h"

#include <string.h>

namespace tl
{

VertexBuffer::VertexBuffer() : Buffer(), hasIdxBuffer(false) { }

VertexBuffer::VertexBuffer(std::vector<Vertex>& vertices)
    : Buffer(), hasIdxBuffer(false)
{
    initBuffer(vertices);
}

VertexBuffer::VertexBuffer(std::vector<Vertex>& vertices, const std::map<std::string, uint8_t>& _attributes)
    : Buffer(), attributes(_attributes), hasIdxBuffer(false)
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

void VertexBuffer::vertexAttributes(int stride)
{
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));

    // normal attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(7 * sizeof(float)));

    // uvs attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)(10 * sizeof(float)));

    // Dynamic attributes
    int sum = 10;
	int index = 4;

    for(auto& [k, v] : attributes) {

        sum += v;
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, v, GL_FLOAT, GL_FALSE, stride, (void*)(sum * sizeof(float)));

        index++;
    }
}

void VertexBuffer::initBuffer(std::vector<Vertex>& vertices, const std::vector<unsigned int> indices)
{
    if (vertices.empty())
        return;

    // Load vertices
    unsigned int index = 0;
    constexpr size_t fixedSize = 12;
    size_t length = fixedSize + vertices[0].scalars.size();

    std::cout << "Vertex length: " << length << std::endl;

    float* glVertices = new float[vertices.size() * length];

    for (Vertex& vertex : vertices) {

        // Fixed
        glVertices[index++] = vertex.position[0];  glVertices[index++] = vertex.position[1];   glVertices[index++] = vertex.position[2];
        glVertices[index++] = vertex.color[0];     glVertices[index++] = vertex.color[1];      glVertices[index++] = vertex.color[2];     glVertices[index++] = vertex.color[3];
        glVertices[index++] = vertex.normal[0];    glVertices[index++] = vertex.normal[1];     glVertices[index++] = vertex.normal[2];
        glVertices[index++] = vertex.uvs[0];       glVertices[index++] = vertex.uvs[1];

        // Scalar fields
        for(float v : vertex.scalars)
            glVertices[index++] = v;
    }

    for(int i = 0; i < length; i ++)
    {
        std::cout << glVertices[i] << " ";
    }
    std::cout << std::endl;

    int stride = length * sizeof(float);

    // Vertex buffer
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * stride, glVertices, GL_DYNAMIC_DRAW);
    delete[] glVertices;

    // Index Buffer
    if (hasIdxBuffer) indexBuffer = IndexBuffer::New(indices);

    // Vertex Attributes
    vertexAttributes(stride);

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
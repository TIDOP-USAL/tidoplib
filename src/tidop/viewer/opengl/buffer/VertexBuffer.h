#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "Buffer.h"

#include "tidop/viewer/Vertex.h"
#include "IndexBuffer.h"

namespace tl
{

class VertexBuffer
    : public Buffer
{
    GENERATE_SHARED_PTR(VertexBuffer)

protected:

    IndexBuffer::Ptr indexBuffer;
    bool hasIdxBuffer;

public:

    VertexBuffer();
    VertexBuffer(std::vector<Vertex>& vertices);
    VertexBuffer(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    VertexBuffer(const VertexBuffer& vertexBuffer);
    VertexBuffer(VertexBuffer&& vertexBuffer) noexcept;
    VertexBuffer& operator=(const VertexBuffer& vertexBuffer);
    VertexBuffer& operator=(VertexBuffer&& vertexBuffer) noexcept;
    ~VertexBuffer();

protected:

    void vertexAttributes();
    void initBuffer(std::vector<Vertex>& vertices, const std::vector<unsigned int> indices = {});

public:

    void bind() override;
    void unbind() override;

public:

    bool hasIndexBuffer() const { return hasIdxBuffer; }
};

}
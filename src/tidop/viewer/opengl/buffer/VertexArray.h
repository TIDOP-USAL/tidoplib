#pragma once

#include "Buffer.h"

namespace tl
{

class VertexArray
    : public Buffer
{
    GENERATE_SHARED_PTR(VertexArray)

public:

    VertexArray();
    VertexArray(const VertexArray& vertexArray);
    VertexArray(VertexArray&& vertexArray) noexcept;
    VertexArray& operator=(const VertexArray& vertexArray);
    VertexArray& operator=(VertexArray&& vertexArray) noexcept;
    ~VertexArray();

protected:

    void initBuffer();

public:

    void bind() override;
    void unbind() override;
};

}
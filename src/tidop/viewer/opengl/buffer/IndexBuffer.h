#pragma once

#include <iostream>
#include <vector>

#include "Buffer.h"

namespace tl
{

class IndexBuffer : public Buffer {
    GENERATE_SHARED_PTR(IndexBuffer)
public:
    IndexBuffer(const std::vector<unsigned int> indices);
    IndexBuffer() = default;
    ~IndexBuffer();
private:
    void initBuffer(std::vector<unsigned int> indices);
public:
    void bind() override;
    void unbind() override;
};

}
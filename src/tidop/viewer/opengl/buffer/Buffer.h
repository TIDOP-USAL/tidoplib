#pragma once

#include <GL/glew.h>

#include "tidop/core/ptr.h"

namespace tl
{

class Buffer
{

protected:

    unsigned int id;

public:

    Buffer() : id(0) {}
    virtual ~Buffer() = default;

public:

    virtual void bind() = 0;
    virtual void unbind() = 0;

public:

    unsigned int getID() const { return id; }
};

}
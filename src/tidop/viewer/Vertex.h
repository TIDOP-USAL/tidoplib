#pragma once

#include "tidop/math/algebra/vector.h"

namespace tl
{

struct Vertex
{
    using Vector3f = tl::Vector3f;
    using Vector4f = tl::Vector4f;

    Vector3f position;
    Vector4f color;

    Vertex(const Vector3f& _position, const Vector4f& _color)
        : position(_position), color(_color) {
    }

    Vertex() = default;

    ~Vertex() = default;
};

}
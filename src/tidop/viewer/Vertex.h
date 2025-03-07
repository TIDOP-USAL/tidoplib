#pragma once

#include "tidop/math/algebra/vector.h"

namespace tl
{

struct Vertex
{
    Vector3f position;
    Vector4f color;
    Vector3f normal;
    Vector2f uvs;

    std::vector<float> scalars;

    Vertex(const Vector3f& _position, const Vector4f& _color)
        : position(_position), color(_color), normal(Vector3f::zero()), uvs(Vector2f::zero()) {
    }

    Vertex(const Vector3f& _position, const Vector4f& _color, const std::vector<float>& _scalars)
        : position(_position), color(_color), normal(Vector3f::zero()), uvs(Vector2f::zero()), scalars(_scalars) {
    }

    Vertex(const Vector3f& _position, const Vector4f& _color, const Vector3f& _normal, const Vector2f& _uvs)
        : position(_position), color(_color), normal(_normal), uvs(_uvs) {
    }

    Vertex() = default;

    ~Vertex() = default;

    void swapYZ()
    {
        const float y0 = position.y();
        position.x() *= -1;
        position.y() = position.z();
        position.z() = y0;
    }
};

}
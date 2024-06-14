#pragma once

#include "matrix.h"

namespace tl
{

struct Matrices 
{

    static auto scale(float sx, float sy, float sz) -> Matrix4x4f
    {
        return Matrix4x4f
        {
            sx, 0.0f, 0.0f, 0.0f,
            0.0f,  sy, 0.0f, 0.0f,
            0.0f, 0.0f,  sz, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    static auto translate(float tx, float ty, float tz) -> Matrix4x4f
    {
        return Matrix4x4f
        {
            1.0f, 0.0f, 0.0f,  tx,
            0.0f, 1.0f, 0.0f,  ty,
            0.0f, 0.0f, 1.0f,  tz,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    static auto rotationX(float angle) -> Matrix4x4f
    {
        return Matrix4x4f
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cos(angle), -sin(angle), 0.0f,
            0.0f, sin(angle), cos(angle), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    static auto rotationY(float angle) -> Matrix4x4f
    {
        return Matrix4x4f
        {
            cos(angle), 0.0f, sin(angle), 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -sin(angle), 0.0f, cos(angle), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    static auto rotationZ(float angle) -> Matrix4x4f
    {
        return Matrix4x4f
        {
            cos(angle), sin(angle), 0.0f, 0.0f,
            -sin(angle), cos(angle), 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    // Right handed perspective projection matrix
    static auto perspective(float fovy, float aspect, float nearZ, float farZ) -> Matrix4x4f
    {
        float a = 1 / (aspect * tan(fovy / 2));
        float b = 1 / tan(fovy / 2);
        float c = -(farZ + nearZ) / (farZ - nearZ);
        float d = -1.0f;
        float e = (-2.0f * farZ * nearZ) / (farZ - nearZ);

        // Row-major order
        return Matrix4x4f
        {
            a , 0.0f, 0.0f, 0.0f,
            0.0f, b,  0.0f, 0.0f,
            0.0f, 0.0f, c,   e,
            0.0f, 0.0f, d,  0.0f
        };  
    }

    // Right handed look at matrix
    static auto lookAt(const Vector3f& position, const Vector3f& target, const Vector3f& up) -> Matrix4x4f
    {
        auto dot = [&](const Vector3f& u, const Vector3f& v) {
            return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
        };

        auto cross = [&](const Vector3f& u, const Vector3f& v) {
            return Vector3f{ u.y() * v.z() - u.z() * v.y(), -u.x() * v.z() + u.z() * v.x(), u.x() * v.y() - u.y() * v.x() };
        };

        Vector3f zaxis = Vector3f({ position.x() - target.x(), position.y() - target.y(), position.z() - target.z()});
        zaxis.normalize();

        Vector3f xaxis = cross(up, zaxis);
        xaxis.normalize();

        Vector3f yaxis = cross(zaxis, xaxis);

        return Matrix4x4f
        {
            xaxis.x() , yaxis.x(), zaxis.x(), 0.0f,
            xaxis.y(),  yaxis.y(), zaxis.y(), 0.0f,
            xaxis.z(),  yaxis.z(), zaxis.z(), 0.0f,
            -dot(xaxis, position), -dot(yaxis, position), -dot(zaxis, position), 1.0f
        }.transpose();
    }
};

}
#pragma once

#include "matrix.h"

namespace tl
{

struct Matrices 
{

    static Matrix4x4f scale(float sx, float sy, float sz)
    {
        return Matrix4x4f
        {
            sx, 0.0f, 0.0f, 0.0f,
            0.0f,  sy, 0.0f, 0.0f,
            0.0f, 0.0f,  sz, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    static Matrix4x4f translate(float tx, float ty, float tz)
    {
        return Matrix4x4f
        {
            1.0f, 0.0f, 0.0f,  tx,
            0.0f, 1.0f, 0.0f,  ty,
            0.0f, 0.0f, 1.0f,  tz,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    static Matrix4x4f rotationX(float angle)
    {
        return Matrix4x4f
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cos(angle), -sin(angle), 0.0f,
            0.0f, sin(angle), cos(angle), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    static Matrix4x4f rotationY(float angle)
    {
        return Matrix4x4f
        {
            cos(angle), 0.0f, sin(angle), 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -sin(angle), 0.0f, cos(angle), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    static Matrix4x4f rotationZ(float angle)
    {
        return Matrix4x4f
        {
            cos(angle), sin(angle), 0.0f, 0.0f,
            -sin(angle), cos(angle), 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    static Matrix4x4f perspective(float fovy, float aspect, float nearZ, float farZ)
    {
        float m11 = 1 / (aspect * tan(fovy / 2));
        float m22 = 1 / tan(fovy / 2);
        float m33 = (-nearZ - farZ) / (nearZ - farZ);
        float m34 = (2 * nearZ * farZ) / (nearZ - farZ);

        return Matrix4x4f
        {
            m11 , 0.0f, 0.0f, 0.0f,
            0.0f,  m22, 0.0f, 0.0f,
            0.0f, 0.0f,  m33,  m34,
            0.0f, 0.0f,  1.0f, 0.0f
        };
    }

    static Matrix4x4f lookAt(const Vector3f& position, const Vector3f& target, const Vector3f& up) {

        auto dot = [&](const Vector3f& u, const Vector3f& v) {
            return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
        };

        auto cross = [&](const Vector3f& u, const Vector3f& v) {
            return Vector3f{ u.y() * v.z() - u.z() * v.y(), -u.x() * v.z() + u.z() * v.x(), u.x() * v.y() - u.y() * v.x() };
        };

        Vector3f zaxis = { position.x() - target.x(), position.y() - target.y(), position.z() - target.z() };
        zaxis.normalize();

        Vector3f xaxis = cross(up, zaxis);
        xaxis.normalize();

        Vector3f yaxis = cross(zaxis, xaxis);

        return Matrix4x4f{
            xaxis.x(), xaxis.y(), xaxis.z(), -dot(xaxis, position),
            yaxis.x(), yaxis.y(), yaxis.z(), -dot(yaxis, position),
            zaxis.x(), zaxis.y(), zaxis.z(), -dot(zaxis, position),
            0, 0, 0, 1
        };
    }

};

}
#include "TrackballCamera.h"

#include <tidop/math/algebra/matrices.h>

namespace tl
{

TrackballCamera::TrackballCamera(const tl::Matrix4x4f& projectionMatrix, const tl::Matrix4x4f& viewMatrix)
    : Camera(projectionMatrix, viewMatrix), theta(tl::consts::pi<float>), phi(tl::consts::two_pi<float>), radius(1.0f) {
}

TrackballCamera::Ptr TrackballCamera::orthoCamera(float left, float right, float bottom, float top, float zNear, float zFar)
{
    tl::Matrix4x4f projection = tl::Matrix4x4f::identity();
    tl::Matrix4x4f view = tl::Matrix4x4f::identity();

    return TrackballCamera::New(projection, view);
}

TrackballCamera::Ptr TrackballCamera::orthoCamera(const Rectf& rect, float zNear, float zFar)
{
    return orthoCamera(rect.x, rect.x + rect.width, rect.y + rect.height, rect.y, zNear, zFar);
}

TrackballCamera::Ptr TrackballCamera::perspectiveCamera(float fovy, float aspect, float zNear, float zFar)
{
    tl::Matrix4x4f projection = tl::Matrices::perspective(fovy, aspect, zNear, zFar);
    tl::Matrix4x4f view = tl::Matrix4x4f::identity();

    TrackballCamera::Ptr camera = TrackballCamera::New(projection, view);
    camera->lookAt(tl::Vector3f({ 0.0f, 0.0f, -1.0f }), tl::Vector3f({ 0.0f, 0.0f, 0.0f }), tl::Vector3f({ 0.0f, 1.0f, 0.0f }));

    return camera;
}

tl::Vector4f TrackballCamera::toCartesianCoords()
{
    return tl::Vector4f({
        radius * sinf(phi) * sinf(theta),
        radius * cosf(phi),
        radius * sinf(phi) * cosf(theta),
        1
    });
}

tl::Vector3f TrackballCamera::getCameraPosition()
{
    tl::Vector4f cartesianCoords = toCartesianCoords();
    tl::Vector3f cameraPosition({ target.x() + cartesianCoords.x(), target.y() + cartesianCoords.y(), target.z() + cartesianCoords.z() });
    return cameraPosition;
}

tl::Matrix4x4f& TrackballCamera::getViewMatrix()
{
    eye = getCameraPosition();
    lookAt(eye, target, up);
    return viewMatrix;
}

void TrackballCamera::rotate(float dTheta, float dPhi)
{
    if (up.y() > 0.0f)    theta += dTheta;
    else                theta -= dTheta;
    phi += dPhi;

    // If phi is between 0 to PI or -PI to -2PI, make 'up' be positive Y, other wise make it negative Y
    constexpr float PI2 = tl::consts::two_pi<float>;
    constexpr float PI = tl::consts::pi<float>;

    if (phi > PI2) phi -= PI2;
    else if (phi < -PI2) phi += PI2;

    // If phi is between 0 to PI or -PI to -2PI, make 'up' be positive Y, other wise make it negative Y
    if ((phi > 0 && phi < PI) || (phi < -PI && phi > -PI2)) up.y() = 1.0f;
    else  up.y() = -1.0f;
}

void TrackballCamera::pan(float dx, float dy)
{
    tl::Matrix4x4f invView = viewMatrix.inverse();
    tl::Vector4f Dx = invView * tl::Vector4f({ 1.0f, 0.0f, 0.0f, 0.0f });
    tl::Vector4f Dy = invView * tl::Vector4f({ 0.0f, 1.0f, 0.0f, 0.0f });
    tl::Vector4f diff = (Dy * dy - Dx * dx);
    target += tl::Vector3f({ diff.x(), diff.y(), diff.z() });
}

void TrackballCamera::zoom(float dRadius)
{
    radius -= dRadius;
    if (radius <= 0.1) radius = 0.1;
}

}
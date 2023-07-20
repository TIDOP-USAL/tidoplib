#include "TrackballCamera.h"

#include <tidop/math/algebra/matrices.h>

namespace tl
{

TrackballCamera::TrackballCamera(const tl::Matrix4x4f& projectionMatrix, const tl::Matrix4x4f& viewMatrix)
    : Camera(projectionMatrix, viewMatrix), theta(tl::consts::pi<float>), phi(tl::consts::two_pi<float>), radius(0.0f) {
}

TrackballCamera TrackballCamera::orthoCamera(float left, float right, float bottom, float top, float zNear, float zFar) {
    tl::Matrix4x4f projection = tl::Matrix4x4f::identity();
    tl::Matrix4x4f view = tl::Matrix4x4f::identity();
    TrackballCamera camera(projection, view);
    return camera;
}

TrackballCamera TrackballCamera::orthoCamera(const Rectf& rect, float zNear, float zFar) {
    return orthoCamera(rect.x, rect.x + rect.width, rect.y + rect.height, rect.y, zNear, zFar);
}

TrackballCamera TrackballCamera::perspectiveCamera(float fovy, float aspect, float zNear, float zFar) {
    tl::Matrix4x4f projection = tl::Matrices::perspective(fovy, aspect, zNear, zFar);
    tl::Matrix4x4f view = tl::Matrix4x4f::identity();
    TrackballCamera camera(projection, view);
    camera.lookAt(tl::Vector3f({ 0.0f, 0.0f, 1.0f }), tl::Vector3f({ 0.0f, 0.0f, 0.0f }), tl::Vector3f({ 0.0f, 1.0f, 0.0f }));
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
    tl::Matrix3x3f invView = viewMatrix.inverse();
    tl::Vector3f Dx = invView * tl::Vector3f({ 1.0f, 0.0f, 0.0f });
    tl::Vector3f Dy = invView * tl::Vector3f({ 0.0f, 1.0f, 0.0f });
    target += (Dy * dy - Dx * dx);
}

void TrackballCamera::zoom(float dRadius)
{
    radius -= dRadius;
    if (radius <= 0.1) radius = 0.1;
}

}
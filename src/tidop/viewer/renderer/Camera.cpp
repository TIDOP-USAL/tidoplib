#include "Camera.h"

namespace tl
{

Camera::Camera(const tl::Matrix4x4f& _projectionMatrix, const tl::Matrix4x4f& _viewMatrix)
    : projectionMatrix(_projectionMatrix), viewMatrix(_viewMatrix), eye({ 0.0f, 0.0f, 0.0f }),
    target({ 0.0f, 0.0f, 0.0f }), up({ 0.0f, 1.0f, 0.0f }) {
}

void Camera::lookAt(const tl::Vector3f& eye, const tl::Vector3f& center, const tl::Vector3f& up) {
    this->eye = eye;
    this->target = center;
    this->up = up;
    viewMatrix = tl::Matrices::lookAt(eye, center, up);
}

}
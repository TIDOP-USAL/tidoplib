#pragma once

#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/matrices.h"

namespace tl
{

class Camera
{

protected:

	tl::Matrix4x4f projectionMatrix;
	tl::Matrix4x4f viewMatrix;
	tl::Vector3f eye, target, up;

public:

	Camera(const tl::Matrix4x4f& _projectionMatrix, const tl::Matrix4x4f& _viewMatrix);
	Camera() = default;
	virtual ~Camera() = default;

public:

	virtual void lookAt(const tl::Vector3f& eye, const tl::Vector3f& center, const tl::Vector3f& up);

public:

	tl::Matrix4x4f& getProjectionMatrix() { return projectionMatrix; }
	tl::Matrix4x4f getInverseProjectionMatrix() { return projectionMatrix.inverse(); }

	virtual tl::Matrix4x4f& getViewMatrix() { return viewMatrix; }
	tl::Matrix4x4f getInverseViewMatrix() { return viewMatrix.inverse(); }
};

}
#pragma once

#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/matrices.h"

#include "tidop/core/base/ptr.h"

namespace tl
{

class Camera
{
	GENERATE_SHARED_PTR(Camera)
protected:

	Matrix4x4f projectionMatrix;
	Matrix4x4f viewMatrix;
	Vector3f eye, target, up;

public:

	Camera(const Matrix4x4f& _projectionMatrix, const Matrix4x4f& _viewMatrix);
	Camera() = default;
	virtual ~Camera() = default;

public:

	virtual void lookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up);

public:

	Matrix4x4f& getProjectionMatrix() { return projectionMatrix; }
	Matrix4x4f getInverseProjectionMatrix() { return projectionMatrix.inverse(); }

	virtual Matrix4x4f& getViewMatrix() { return viewMatrix; }
	Matrix4x4f getInverseViewMatrix() { return viewMatrix.inverse(); }

	void setEye(const Vector3f& eye) { this->eye = eye; }
	Vector3f& getEye() { return eye; }

	void setTarget(const Vector3f& target) { this->target = target; }
	Vector3f& getTarget() { return target; }

	void setUp(const Vector3f& up) { this->up = up; }
	Vector3f& getUp() { return up; }
};

}
#pragma once

#include "Camera.h"

#include "tidop/geometry/rect.h"

namespace tl
{

class TrackballCamera
	: public Camera {
	GENERATE_SHARED_PTR(TrackballCamera)
private:

	float theta, phi;
	float radius;

public:

	TrackballCamera(const tl::Matrix4x4f& projectionMatrix, const tl::Matrix4x4f& viewMatrix);

private:

	tl::Vector4f toCartesianCoords();
	tl::Vector3f getCameraPosition();

public:

	static TrackballCamera::Ptr orthoCamera(float left, float right, float bottom, float top, float zNear, float zFar);
	static TrackballCamera::Ptr orthoCamera(const Rectf& rect, float zNear, float zFar);

	static TrackballCamera::Ptr perspectiveCamera(float fovy, float aspect, float zNear, float zFar);

	virtual tl::Matrix4x4f& getViewMatrix() override;

	void rotate(float dTheta, float dPhi);
	void pan(float dx, float dy);
	void zoom(float dRadius);

	void setTheta(float theta) { this->theta = theta; }
	float getTheta() const { return theta; }

	void setPhi(float phi) { this->phi = phi; }
	float getPhi() const { return phi; }

	void setRadius(float radius) { this->radius = radius; }
	float getRadius() const { return radius; }
};

}
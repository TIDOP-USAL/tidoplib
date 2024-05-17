#pragma once

#include "tidop/math/algebra/vector.h"
#include "tidop/math/geometry/analytic/line.h"
#include "tidop/viewer/renderer/Camera.h"

namespace tl
{

class RayCaster
{

protected:

	Camera::Ptr camera;
	unsigned int width, height;

public:

	RayCaster(const Camera::Ptr& _camera, unsigned int _width, unsigned int _height)
		: camera(_camera), width(_width), height(_height) {
	}

	RayCaster() = default;
	~RayCaster() = default;

protected:

	Vector2f normalizeDeviceCoords(const Vector2i& mousePosition)
	{
		float x = (2.0f * mousePosition.x()) / width - 1.0f;
		float y = (2.0f * mousePosition.y()) / height - 1.0f;
		return { x, y };
	}

	Vector4f homogeneousClipCoords(const Vector2f& normalizedDeviceCoords)
	{
		return { normalizedDeviceCoords.x(), normalizedDeviceCoords.y(), -1.f, 1.f};
	}

	Vector4f eyeCoords(const Vector4f& clipCoords)
	{
		Vector4f eyeCoords = camera->getInverseProjectionMatrix() * clipCoords;
		return { eyeCoords.x(), eyeCoords.y(), -1.f, 0.f };
	}

	Vector3f worldCoords(const Vector4f& eyeCoords)
	{
		Vector4f worldCoordinates = camera->getInverseViewMatrix() * eyeCoords;
		Vector3f worldCoordinates3({ worldCoordinates.x(), worldCoordinates.y(), worldCoordinates.z()});
		worldCoordinates3.normalize();
		return worldCoordinates3;
	}

public:

	Line<float> ray(const Vector2i& mousePosition)
	{
		Vector2f normalizedDeviceCoords = normalizeDeviceCoords(mousePosition);
		normalizedDeviceCoords.y() *= -1;
		Vector4f clipCoords = homogeneousClipCoords(normalizedDeviceCoords);
		Vector4f eye = eyeCoords(clipCoords);
		Vector3f world = worldCoords(eye);
		return Line(camera->getEye(), world);
	}

public:

	Camera::Ptr getCamera() { return camera; }

	unsigned int getWidth() const { return width; }
	unsigned int getHeight() const { return height; }
};

}

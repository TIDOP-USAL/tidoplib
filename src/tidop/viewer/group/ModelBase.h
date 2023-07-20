#pragma once

#include <iostream>
#include <vector>

#include "tidop/viewer/opengl/buffer/VertexArray.h"
#include "tidop/viewer/opengl/buffer/VertexBuffer.h"

#include <tidop/math/algebra/vector.h>
#include <tidop/math/algebra/matrix.h>
#include <tidop/math/algebra/axis_angle.h>

namespace tl
{

class ModelBase
{
protected:

	VertexArray::Ptr vertexArray;
	VertexBuffer::Ptr vertexBuffer;

	tl::Matrix4x4f modelMatrix;

	unsigned int pointSize;

public:

	ModelBase(std::vector<Vertex>& points, const std::vector<unsigned int> indices = {});
	ModelBase() = default;
	virtual ~ModelBase() = default;

public:

	void translate(float tx, float ty, float tz) {

	}

	void rotate(const tl::AxisAngle<float>& axis) {

	}

	void scale(float sx, float sy, float sz) {

	}
};
}
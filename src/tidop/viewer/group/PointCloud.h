#pragma once

#include <iostream>
#include <vector>

#include "tidop/viewer/opengl/buffer/VertexArray.h"
#include "tidop/viewer/opengl/buffer/VertexBuffer.h"

#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/matrix.h"


namespace tl 
{

class PointCloud
{

protected:

	VertexArray::Ptr vertexArray;
	VertexBuffer::Ptr vertexBuffer;

	Matrix4x4f modelMatrix;

	unsigned int pointSize;

public:

	PointCloud(std::vector<Vertex>& points);
};

}
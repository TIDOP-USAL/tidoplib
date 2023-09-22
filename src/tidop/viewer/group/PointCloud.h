#pragma once

#include <iostream>
#include <vector>

#include "tidop/viewer/opengl/buffer/VertexArray.h"
#include "tidop/viewer/opengl/buffer/VertexBuffer.h"

#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/matrix.h"

#include "tidop/viewer/group/ModelBase.h"

namespace tl 
{

class PointCloud
	: public ModelBase {

	GENERATE_SHARED_PTR(PointCloud)

private:

	size_t size;

public:

	PointCloud(std::vector<Vertex>& points);
	PointCloud(size_t _size);
	PointCloud() = default;
	~PointCloud() = default;

public:

	void draw() override;

};

}
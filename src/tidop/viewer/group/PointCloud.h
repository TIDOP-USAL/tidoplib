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

public:

	PointCloud(const std::vector<Vertex>& points);
	PointCloud(const std::vector<Vertex>& points, const std::map<std::string, uint8_t>& attributes);
	PointCloud(size_t _size);
	PointCloud() = default;
	~PointCloud() = default;

};

}
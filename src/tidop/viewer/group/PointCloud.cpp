#include "PointCloud.h"

namespace tl
{

PointCloud::PointCloud(const std::vector<Vertex>& points)
	: ModelBase(points, Type::PointCloud) {
}

PointCloud::PointCloud(size_t _size)
	: ModelBase(_size, Type::PointCloud) {
}

}
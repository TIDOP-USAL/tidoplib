#include "PointCloud.h"

namespace tl
{

PointCloud::PointCloud(const std::vector<Vertex>& points)
	: ModelBase(points, Type::PointCloud) {
}

PointCloud::PointCloud(const std::vector<Vertex>& points, const std::map<std::string, uint8_t>& attributes)
	: ModelBase(points, attributes, Type::PointCloud) {
}

PointCloud::PointCloud(size_t _size)
	: ModelBase(_size, Type::PointCloud) {
}

}
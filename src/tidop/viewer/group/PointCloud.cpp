#include "PointCloud.h"

namespace tl
{

PointCloud::PointCloud(std::vector<Vertex>& points)
	: ModelBase(points, Type::PointCloud), size(points.size()) {
}

PointCloud::PointCloud(size_t _size)
	: ModelBase(_size, Type::PointCloud), size(_size) {
}

void PointCloud::draw()
{
	glPointSize(pointSize);
	vertexArray->bind();
	glDrawArrays(static_cast<int>(type), 0, size);
	vertexArray->unbind();
}

}
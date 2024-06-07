#include "Grid.h"

namespace tl
{

Grid::Grid(const Vector3f& _origin, const Size<int>& _dim, float _squareSize)
	: ModelBase(Type::MultiLine), origin(_origin), dim(_dim), squareSize(_squareSize) {
	computeGrid();
}

Grid::Grid(const Grid& grid)
	: ModelBase(grid), origin(grid.origin), dim(grid.dim), squareSize(grid.squareSize) {
}

Grid::Grid(Grid&& grid) noexcept
	: ModelBase(std::forward<ModelBase>(grid)), origin(std::move(grid.origin)), dim(std::move(grid.dim)), squareSize(grid.squareSize) {
}

Grid& Grid::operator=(const Grid& grid)
{
	ModelBase::operator=(grid);
	origin = grid.origin;
	dim = grid.dim;
	squareSize = grid.squareSize;
	return *this;
}

Grid& Grid::operator=(Grid&& grid) noexcept
{
	ModelBase::operator=(std::forward<ModelBase>(grid));
	origin = std::move(grid.origin);
	dim = std::move(grid.dim);
	squareSize = grid.squareSize;
	return *this;
}

void Grid::computeGrid()
{
	Vector3f min{3, std::numeric_limits<float>::infinity()};
	Vector3f max = Vector3f::zero();

	const float totalWidth = static_cast<float>(dim.width) * squareSize;
	const float totalDepth = static_cast<float>(dim.height) * squareSize;

	for(int i = 0; i <= dim.width; i ++)
	{
		float x = origin.x() + i * squareSize - totalWidth / 2.0f;
		points.emplace_back(Vector3f{ x, 0, -totalDepth / 2.0f }, Vector4f{ 0.1f, 0.1f, 0.1f, 1.0f });
		points.emplace_back(Vector3f{ x, 0, totalDepth / 2.0f }, Vector4f{ 0.1f, 0.1f, 0.1f, 1.0f });
	}

	for(int k = 0; k <= dim.height; k ++)
	{
		float z = origin.z() + k * squareSize - totalDepth / 2.0f;
		points.emplace_back(Vector3f{ -totalWidth / 2.0f, 0, z }, Vector4f{ 0.1f, 0.1f, 0.1f, 1.0f });
		points.emplace_back(Vector3f{ totalWidth /  2.0f, 0, z }, Vector4f{ 0.1f, 0.1f, 0.1f, 1.0f });
	}

	vertexArray = VertexArray::New();
	vertexBuffer = VertexBuffer::New(points);
}

void Grid::draw()
{
	vertexArray->bind();
	glLineWidth(lineSize);
	glDrawArrays(static_cast<int>(type), 0, points.size());
	glLineWidth(1.0);
	vertexArray->unbind();
}

}
#pragma once

#include "ModelBase.h"

namespace tl
{

class Grid : public ModelBase
{
	GENERATE_SHARED_PTR(Grid)

private:

	Vector3f origin;
	Size<int> dim;
	float squareSize;

public:

	Grid(const Vector3f& _origin,const Size<int>& _dim, float _squareSize);
	Grid() = default;
	~Grid() = default;
	Grid(const Grid& grid);
	Grid(Grid&& grid) noexcept;
	Grid& operator=(const Grid& grid);
	Grid& operator=(Grid&& grid) noexcept;

private:

	void computeGrid();

public:

	void draw() override;

public:
	Vector3f getOrigin() const { return origin; }
	Size<int> getDimensions() const { return dim; }
	float getSquareSize() const { return squareSize; }
};

}
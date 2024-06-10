#pragma once

#include "tidop/math/algebra/vector.h"

namespace tl
{

template <typename T>
class Line
{
private:
	Vector<T, 3> origin;
	Vector<T, 3> direction;

public:
	Line(const Vector<T, 3>& _origin, const Vector<T, 3>& _direction)
		: origin(_origin), direction(_direction) {
	}

	Line() = default;
	~Line() = default;

public:
	Vector<T, 3> evaluate(T lambda)
	{
		return origin + direction * lambda;
	}

	T distance(const Vector<T, 3>& point)
	{
		T numModule = crossProduct(point - origin, direction).module();
		T directionModule = direction.module();
		return numModule / directionModule;
	}

public:
	Vector<T, 3> getOrigin() const { return origin; }
	Vector<T, 3> getDirection() const { return direction; }

};

}
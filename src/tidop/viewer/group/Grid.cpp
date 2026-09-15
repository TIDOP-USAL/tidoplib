/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/


#include "tidop/viewer/group/Grid.h"

namespace tl
{

Grid::Grid(Vector3f _origin, Size<int> _dim, float _squareSize)
  : ModelBase(Type::MultiLine),
    origin(std::move(_origin)),
    dim(std::move(_dim)),
    squareSize(_squareSize)
{
    computeGrid();
}

//Grid::Grid(const Grid& grid)
//    : ModelBase(grid), origin(grid.origin), dim(grid.dim), squareSize(grid.squareSize) {
//}
//
//Grid::Grid(Grid&& grid) noexcept
//    : ModelBase(std::forward<ModelBase>(grid)), origin(std::move(grid.origin)), dim(std::move(grid.dim)), squareSize(grid.squareSize) {
//}
//
//Grid& Grid::operator=(const Grid& grid)
//{
//    ModelBase::operator=(grid);
//    origin = grid.origin;
//    dim = grid.dim;
//    squareSize = grid.squareSize;
//    return *this;
//}
//
//Grid& Grid::operator=(Grid&& grid) noexcept
//{
//    ModelBase::operator=(std::forward<ModelBase>(grid));
//    origin = std::move(grid.origin);
//    dim = std::move(grid.dim);
//    squareSize = grid.squareSize;
//    return *this;
//}

void Grid::computeGrid()
{
    //Vector3f min{3, std::numeric_limits<float>::infinity()};
    //Vector3f max = Vector3f::zero();

    const float totalWidth = static_cast<float>(dim.width()) * squareSize;
    const float totalDepth = static_cast<float>(dim.height()) * squareSize;
    const float halfWidth = totalWidth * 0.5f;
    const float halfDepth = totalDepth * 0.5f;

    const Vector4f gridColor{0.1f, 0.1f, 0.1f, 1.0f};

    const size_t totalVertices = static_cast<size_t>(dim.width() + 1 + dim.height() + 1) * 2;
    points.clear();
    points.reserve(totalVertices);

    for(int i = 0; i <= dim.width(); i ++)
    {
        float x = origin.x() + static_cast<float>(i) * squareSize - halfWidth;
        points.emplace_back(Vector3f{ x, 0, -halfDepth}, gridColor);
        points.emplace_back(Vector3f{ x, 0, halfDepth}, gridColor);
    }

    for(int k = 0; k <= dim.height(); k ++)
    {
        float z = origin.z() + static_cast<float>(k) * squareSize - halfDepth;
        points.emplace_back(Vector3f{ -halfWidth, 0, z }, gridColor);
        points.emplace_back(Vector3f{ halfWidth, 0, z }, gridColor);
    }

    vertexArray = VertexArray::New();
    vertexBuffer = VertexBuffer::New(points);

    length = points.size() / 2;
}

void Grid::draw()
{
    if (!vertexArray || points.empty()) {
        return;
    }

    vertexArray->bind();
    glLineWidth(lineSize);
    glDrawArrays(static_cast<GLenum>(type), 0, static_cast<GLsizei>(points.size()));
    glLineWidth(1.0f);
    vertexArray->unbind();
}

}
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

#pragma once

#include "tidop/viewer/group/ModelBase.h"

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

    Grid() = default;
    Grid(Vector3f _origin, Size<int> _dim, float _squareSize);
    //Grid(const Grid& grid) = default;
    Grid(Grid&& grid) noexcept = default;

    ~Grid() override = default;

    //Grid& operator=(const Grid& grid) = default;
    Grid& operator=(Grid&& grid) noexcept = default;

    void draw() override;

    [[nodiscard]] 
    auto getOrigin() const -> Vector3f { return origin; }
    
    [[nodiscard]] 
    auto getDimensions() const -> Size<int> { return dim; }
    
    [[nodiscard]] 
    auto getSquareSize() const -> float { return squareSize; }

private:

    void computeGrid();
};

}
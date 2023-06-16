/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
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


#include "tidop/core/messages.h"
#include "tidop/geometry/transform/transform.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/math/algebra/rotation_matrix.h"
#include "tidop/math/mathutils.h"

namespace tl
{

/*!
 * \addtogroup geospatial
 *
 * \{
 */


class TL_EXPORT DifferentialRectification
{

private:

    RotationMatrix<double> mRotationMatrix;
    Point3<double> mCameraPosition;
    double mFocal;

public:

    DifferentialRectification();
    DifferentialRectification(const RotationMatrix<double> &rotationMatrix,
                              const Point3<double> &cameraPosition,
                              double focal);
    ~DifferentialRectification();

    auto rotationMatrix() const -> RotationMatrix<double>;
    auto setRotationMatrix(const RotationMatrix<double> &rotationMatrix) -> void;
    auto cameraPosition() const -> Point3<double>;
    auto setCameraPosition(const Point3<double> &cameraPosition) -> void;
    auto focal() const -> double;
    auto setFocal(double focal) -> void;

    auto forwardProjection(const Point<double> &imagePoint, double z) const -> Point3<double>;
    auto backwardProjection(const Point3<double> &groundPoint) const -> Point<double>;

};

/*! \} */ // end of geospatial

} // End namespace tl

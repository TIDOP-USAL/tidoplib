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

#include "tidop/geospatial/diffrect.h"

namespace tl
{


DifferentialRectification::DifferentialRectification()
  : mRotationMatrix(RotationMatrix<double>::identity()),
    mCameraPosition(1., 1., 1.),
    mFocal(1)
{
}

DifferentialRectification::DifferentialRectification(const RotationMatrix<double> &rotationMatrix,
                                                     const Point3<double> &cameraPosition,
                                                     double focal)
  : mRotationMatrix(rotationMatrix),
    mCameraPosition(cameraPosition),
    mFocal(focal)
{
}

DifferentialRectification::~DifferentialRectification()
{
}

auto DifferentialRectification::rotationMatrix() const -> RotationMatrix<double>
{
    return mRotationMatrix;
}

auto DifferentialRectification::setRotationMatrix(const RotationMatrix<double> &rotationMatrix) -> void
{
    mRotationMatrix = rotationMatrix;
}

auto DifferentialRectification::cameraPosition() const -> Point3<double>
{
    return mCameraPosition;
}

auto DifferentialRectification::setCameraPosition(const Point3<double> &cameraPosition) -> void
{
    mCameraPosition = cameraPosition;
}

auto DifferentialRectification::focal() const -> double
{
    return mFocal;
}

auto DifferentialRectification::setFocal(double focal) -> void
{
    mFocal = focal;
}

auto DifferentialRectification::forwardProjection(const Point<double> &imagePoint, double z) const -> Point3<double>
{
    Point3<double> point;

    try {

        point.z = z;

        double x = imagePoint.x;
        double y = imagePoint.y;
        double z_dif = point.z - mCameraPosition.z;

        double div = mRotationMatrix.at(0, 2) * x +
            mRotationMatrix.at(1, 2) * y -
            mRotationMatrix.at(2, 2) * mFocal;

        point.x = mCameraPosition.x +
            z_dif * (mRotationMatrix.at(0, 0) * imagePoint.x +
                     mRotationMatrix.at(1, 0) * imagePoint.y -
                     mRotationMatrix.at(2, 0) * mFocal) / div;

        point.y = mCameraPosition.y +
            z_dif * (mRotationMatrix.at(0, 1) * imagePoint.x +
                     mRotationMatrix.at(1, 1) * imagePoint.y -
                     mRotationMatrix.at(2, 1) * mFocal) / div;

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return point;
}

auto DifferentialRectification::backwardProjection(const Point3<double> &groundPoint) const -> Point<double>
{
    Point<double> photo_coordinates;

    try {

        double dx = groundPoint.x - mCameraPosition.x;
        double dy = groundPoint.y - mCameraPosition.y;
        double dz = groundPoint.z - mCameraPosition.z;
        double div = mRotationMatrix.at(2, 0) * dx +
                     mRotationMatrix.at(2, 1) * dy +
                     mRotationMatrix.at(2, 2) * dz;


        photo_coordinates.x = -mFocal * (mRotationMatrix.at(0, 0) * dx +
                                         mRotationMatrix.at(0, 1) * dy +
                                         mRotationMatrix.at(0, 2) * dz) / div;
        photo_coordinates.y = -mFocal * (mRotationMatrix.at(1, 0) * dx +
                                         mRotationMatrix.at(1, 1) * dy +
                                         mRotationMatrix.at(1, 2) * dz) / div;

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return photo_coordinates;
}


} // End namespace tl

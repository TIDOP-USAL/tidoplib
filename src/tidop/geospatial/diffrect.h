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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_GEOSPATIAL_DIFFERENTIAL_RECTIFICATION_H
#define TL_GEOSPATIAL_DIFFERENTIAL_RECTIFICATION_H

#include "config_tl.h"

#include "tidop/core/messages.h"
#include "tidop/geometry/transform/transform.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/math/algebra/rotation_matrix.h"
#include "tidop/math/mathutils.h"

namespace tl
{

namespace geospatial
{

template<typename T>
class DifferentialRectification
{

public:

  DifferentialRectification();
  DifferentialRectification(const math::RotationMatrix<T> &rotationMatrix,
                            const Point3<T> &cameraPosition,
                            T focal);
	~DifferentialRectification();

  math::RotationMatrix<T> rotationMatrix() const;
  void setRotationMatrix(const math::RotationMatrix<T> &rotationMatrix) const;
  Point3<T> cameraPosition() const;
  void setCameraPosition(const Point3<T> &cameraPosition) const;
  T focal() const;
  void setFocal(T focal);

  Point3<T> forwardProjection(const Point<T> &imagePoint, T z) const;
  Point<T> backwardProjection(const Point3<T> &groundPoint) const;

private:

  math::RotationMatrix<T> mRotationMatrix;
  Point3<T> mCameraPosition;
  T mFocal;

};

template<typename T> inline
DifferentialRectification<T>::DifferentialRectification()
  : mRotationMatrix(math::RotationMatrix<T>::identity()),
    mCameraPosition(1,1,1),
    mFocal(1)
{
}

template<typename T> inline
DifferentialRectification<T>::DifferentialRectification(const math::RotationMatrix<T> &rotationMatrix,
                                                        const Point3<T> &cameraPosition,
                                                        T focal)
  : mRotationMatrix(rotationMatrix),
    mCameraPosition(cameraPosition),
    mFocal(focal)
{
}

template<typename T> inline
DifferentialRectification<T>::~DifferentialRectification()
{
}

template<typename T>
inline math::RotationMatrix<T> DifferentialRectification<T>::rotationMatrix() const
{
  return mRotationMatrix;
}

template<typename T>
inline void DifferentialRectification<T>::setRotationMatrix(const math::RotationMatrix<T> &rotationMatrix) const
{
  mRotationMatrix = rotationMatrix;
}

template<typename T>
inline Point3<T> DifferentialRectification<T>::cameraPosition() const
{
  return mCameraPosition;
}

template<typename T>
inline void DifferentialRectification<T>::setCameraPosition(const Point3<T> &cameraPosition) const
{
  mCameraPosition = cameraPosition;
}

template<typename T>
inline T DifferentialRectification<T>::focal() const
{
  return mFocal;
}

template<typename T>
inline void DifferentialRectification<T>::setFocal(T focal)
{
  return mFocal;
}

template<typename T>
inline Point3<T> DifferentialRectification<T>::forwardProjection(const Point<T> &imagePoint, T z) const
{
  Point3<T> point;

  try {

    point.z = z;
    
    T x = imagePoint.x;
    T y = imagePoint.y;
    T z_dif = point.z - mCameraPosition.z;
    
    T div = mRotationMatrix.at(0, 2) * x + 
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
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
  
  return point;
}

template<typename T>
inline Point<T> DifferentialRectification<T>::backwardProjection(const Point3<T> &groundPoint) const
{
  Point<T> photo_coordinates;

  try {

    T dx = groundPoint.x - mCameraPosition.x;
    T dy = groundPoint.y - mCameraPosition.y;
    T dz = groundPoint.z - mCameraPosition.z;
    T div = mRotationMatrix.at(2, 0) * dx + 
            mRotationMatrix.at(2, 1) * dy + 
            mRotationMatrix.at(2, 2) * (groundPoint.z - mCameraPosition.z);


    photo_coordinates.x = -mFocal * (mRotationMatrix.at(0, 0) * dx +
                                     mRotationMatrix.at(0, 1) * dy +
                                     mRotationMatrix.at(0, 2) * dz) / div;
    photo_coordinates.y = -mFocal * (mRotationMatrix.at(1, 0) * dx +
                                     mRotationMatrix.at(1, 1) * dy +
                                     mRotationMatrix.at(1, 2) * dz) / div;

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }

  return photo_coordinates;
}


} // End namespace geospatial

} // End namespace tl

#endif // TL_GEOSPATIAL_DIFFERENTIAL_RECTIFICATION_H
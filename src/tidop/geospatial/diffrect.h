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

class TL_EXPORT DifferentialRectification
{

public:

  DifferentialRectification();
  DifferentialRectification(const math::RotationMatrix<double> &rotationMatrix,
                            const Point3<double> &cameraPosition,
                            double focal);
	~DifferentialRectification();

  math::RotationMatrix<double> rotationMatrix() const;
  void setRotationMatrix(const math::RotationMatrix<double> &rotationMatrix);
  Point3<double> cameraPosition() const;
  void setCameraPosition(const Point3<double> &cameraPosition);
  double focal() const;
  void setFocal(double focal);

  Point3<double> forwardProjection(const Point<double> &imagePoint, double z) const;
  Point<double> backwardProjection(const Point3<double> &groundPoint) const;

private:

  math::RotationMatrix<double> mRotationMatrix;
  Point3<double> mCameraPosition;
  double mFocal;

};


} // End namespace geospatial

} // End namespace tl

#endif // TL_GEOSPATIAL_DIFFERENTIAL_RECTIFICATION_H
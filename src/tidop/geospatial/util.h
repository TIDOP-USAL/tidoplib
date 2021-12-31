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

#ifndef TL_GEOSPATIAL_UTIL_H
#define TL_GEOSPATIAL_UTIL_H

#include "tidop/core/defs.h"

//#ifdef TL_HAVE_GDAL
//TL_SUPPRESS_WARNINGS
//#include "ogr_spatialref.h"
//TL_DEFAULT_WARNINGS
//#endif

#include "tidop/geometry/entities/point.h"
#include "tidop/math/algebra/rotation_matrix.h"

namespace tl
{

namespace geospatial
{

TL_EXPORT int utmZoneFromLongitude(double longitude);

TL_EXPORT Point3D projectPhotoToTerrain(const tl::math::RotationMatrix<double> &rotation_matrix,
                              const Point3D &camera_position,
                              const PointD &coordinates_image,
                              double focal,
                              double z);

TL_EXPORT PointD projectTerrainToPhoto(const tl::math::RotationMatrix<double> &rotation_matrix,
                             const Point3D &camera_position,
                             const Point3D &coordinates_terrain,
                             double focal);

} // End namespace geospatial

} // End namespace tl


#endif // TL_GEOSPATIAL_UTIL_H

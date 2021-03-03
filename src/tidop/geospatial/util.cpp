/************************************************************************
 *                                                                      *
 * Copyright (C) 2020 by Tidop Research Group                           *
 *                                                                      *
 * This file is part of TidopLib                                        *
 *                                                                      *
 * TidopLib is free software: you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * TidopLib is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                      *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>                *
 *                                                                      *
 ************************************************************************/

#include "crs.h"
#include "util.h"

namespace tl
{

namespace geospatial
{


int utmZoneFromLongitude(double longitude)
{
  TL_TODO("Esto es el calculo del huso correspondiente a la longitud aunque no tiene porque coincidir con el huso real ya que hay zonas que usan husos extendidos")
  int zone = static_cast<int>(ceil((longitude + 180.) / 6.0));
  if (zone == 0) zone = 1;
  return zone;
}

Point3D projectPhotoToTerrain(const tl::math::RotationMatrix<double> &rotation_matrix, 
                              const Point3D &camera_position, 
                              const PointD &coordinates_image, 
                              double focal,
                              double z)
{
  Point3D point;

  point.z = z;

  double x = coordinates_image.x;
  double y = coordinates_image.y;
  double z_dif = point.z - camera_position.z;

  double div = rotation_matrix.at(0, 2) * x + 
               rotation_matrix.at(1, 2) * y - 
               rotation_matrix.at(2, 2) * focal;

  point.x = camera_position.x + z_dif * (rotation_matrix.at(0, 0) * coordinates_image.x + 
                                         rotation_matrix.at(1, 0) * coordinates_image.y - 
                                         rotation_matrix.at(2, 0) * focal) / div;

  point.y = camera_position.y + z_dif * (rotation_matrix.at(0, 1) * coordinates_image.x + 
                                         rotation_matrix.at(1, 1) * coordinates_image.y - 
                                         rotation_matrix.at(2, 1) * focal) / div;

  return point;
}

PointD projectTerrainToPhoto(const tl::math::RotationMatrix<double> &rotation_matrix,
                             const Point3D &camera_position, 
                             const Point3D &coordinates_terrain, 
                             double focal)
{
  double dx = coordinates_terrain.x - camera_position.x;
  double dy = coordinates_terrain.y - camera_position.y;
  double dz = coordinates_terrain.z - camera_position.z;
  double div = rotation_matrix.at(2, 0) * dx + 
               rotation_matrix.at(2, 1) * dy + 
               rotation_matrix.at(2, 2) * (coordinates_terrain.z - camera_position.z);

  PointD photocoord;
  photocoord.x = -focal * (rotation_matrix.at(0, 0) * dx + 
                           rotation_matrix.at(0, 1) * dy + 
                           rotation_matrix.at(0, 2) * dz) / div;
  photocoord.y = -focal * (rotation_matrix.at(1, 0) * dx + 
                           rotation_matrix.at(1, 1) * dy + 
                           rotation_matrix.at(1, 2) * dz) / div;
  return photocoord;
}


#if defined HAVE_GDAL && defined HAVE_PROJ4



#endif // HAVE_GDAL


} // End namespace  geospatial

} // End namespace tl

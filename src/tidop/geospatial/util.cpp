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

#include "tidop/geospatial/crs.h"
#include "tidop/geospatial/util.h"

namespace tl
{

namespace geospatial
{

constexpr auto utm_latitude_band_letters = "CDEFGHJKLMNPQRSTUVWXX";

int utmZoneFromLongitude(double longitude)
{
  return static_cast<int>(std::floor((longitude + 180.) / 6.)) % 60 + 1;
}

std::pair<int, char> utmZoneFromLonLat(double longitude, double latitude)
{

  if (latitude > -80.0 && latitude <= 84.0) {

    // Wikipedia: On the southwest coast of Norway, grid zone 32V (9° of longitude in width) 
    // is extended further west, and grid zone 31V (3° of longitude in width) is correspondingly 
    // shrunk to cover only open water.

    if (latitude >= 56.0 && latitude < 64.0) {
      if (longitude >= 0.0 && longitude < 3.0)
        return std::make_pair(31, 'V');
      else if (longitude >= 3.0 && longitude < 12.0)
        return std::make_pair(32, 'V');
    }

    // Wikipedia: In the region around Svalbard, the four grid zones 31X (9° of longitude in width), 
    // 33X (12° of longitude in width), 35X (12° of longitude in width), and 37X (9° of longitude in 
    // width) are extended to cover what would otherwise have been covered by the seven grid zones 31X 
    // to 37X. The three grid zones 32X, 34X and 36X are not used.

    if (latitude >= 72.0 && latitude <= 84.0 ) {
      if (longitude >= 0.0 && longitude < 9.0)
        return std::make_pair(31, 'X');
      else if (longitude >= 9.0 && longitude < 21.0)
        return std::make_pair(33, 'X');
      else if (longitude >= 21.0 && longitude < 33.0)
        return std::make_pair(35, 'X');
      else if (longitude >= 33.0 && longitude < 42.0)
        return std::make_pair(37, 'X');
    }
      
    // Caso normal
    int zone = utmZoneFromLongitude(longitude);

    int index = std::floor((latitude + 80.0) / 8.0);
    
    auto latitude_band = utm_latitude_band_letters[index];

    return std::make_pair(zone, latitude_band);

  } else if (latitude > 84.0) {

    // Polo Norte
    if (longitude < 0) return std::make_pair(0, 'Y');
    if (longitude > 0) return std::make_pair(0, 'Z');

  } else if (latitude < -80.0) {

    // Polo Sur
    if (longitude < 0) return std::make_pair(0, 'A');
    if (longitude > 0) return std::make_pair(0, 'B');

  } else {
    throw std::runtime_error("");
  }
}

//Point3<double> projectPhotoToTerrain(const tl::math::RotationMatrix<double> &rotation_matrix, 
//                              const Point3<double> &camera_position, 
//                              const Point<double> &coordinates_image, 
//                              double focal,
//                              double z)
//{
//  Point3<double> point;
//
//  point.z = z;
//
//  double x = coordinates_image.x;
//  double y = coordinates_image.y;
//  double z_dif = point.z - camera_position.z;
//
//  double div = rotation_matrix.at(0, 2) * x + 
//               rotation_matrix.at(1, 2) * y - 
//               rotation_matrix.at(2, 2) * focal;
//
//  point.x = camera_position.x + z_dif * (rotation_matrix.at(0, 0) * coordinates_image.x + 
//                                         rotation_matrix.at(1, 0) * coordinates_image.y - 
//                                         rotation_matrix.at(2, 0) * focal) / div;
//
//  point.y = camera_position.y + z_dif * (rotation_matrix.at(0, 1) * coordinates_image.x + 
//                                         rotation_matrix.at(1, 1) * coordinates_image.y - 
//                                         rotation_matrix.at(2, 1) * focal) / div;
//
//  return point;
//}
//
//Point<double> projectTerrainToPhoto(const tl::math::RotationMatrix<double> &rotation_matrix,
//                             const Point3<double> &camera_position, 
//                             const Point3<double> &coordinates_terrain, 
//                             double focal)
//{
//  double dx = coordinates_terrain.x - camera_position.x;
//  double dy = coordinates_terrain.y - camera_position.y;
//  double dz = coordinates_terrain.z - camera_position.z;
//  double div = rotation_matrix.at(2, 0) * dx + 
//               rotation_matrix.at(2, 1) * dy + 
//               rotation_matrix.at(2, 2) * (coordinates_terrain.z - camera_position.z);
//
//  Point<double> photocoord;
//  photocoord.x = -focal * (rotation_matrix.at(0, 0) * dx + 
//                           rotation_matrix.at(0, 1) * dy + 
//                           rotation_matrix.at(0, 2) * dz) / div;
//  photocoord.y = -focal * (rotation_matrix.at(1, 0) * dx + 
//                           rotation_matrix.at(1, 1) * dy + 
//                           rotation_matrix.at(1, 2) * dz) / div;
//  return photocoord;
//}


#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ4



#endif // TL_HAVE_GDAL


} // End namespace  geospatial

} // End namespace tl

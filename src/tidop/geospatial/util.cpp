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

        if (latitude >= 72.0 && latitude <= 84.0) {
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


#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ



#endif // TL_HAVE_GDAL

} // End namespace tl

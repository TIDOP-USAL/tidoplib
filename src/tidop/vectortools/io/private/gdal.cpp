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

#include "tidop/vectortools/io/private/gdal.h"

#include "tidop/core/base/exception.h"
#include "tidop/core/base/string_utils.h"

namespace tl
{

namespace internal
{


std::string gdalVectorDriverFromExtension(const std::string &extension)
{
    std::string format;
	
    if (compareInsensitiveCase(extension, ".dxf"))
        format = "DXF";
    else if (compareInsensitiveCase(extension, ".dwg"))
        format = "DWG";
    else if (compareInsensitiveCase(extension, ".dgn"))
        format = "DGN";
    else if (compareInsensitiveCase(extension, ".shp"))
        format = "ESRI Shapefile";
    else if (compareInsensitiveCase(extension, ".gml"))
        format = "GML";
    else if (compareInsensitiveCase(extension, ".kml") || compareInsensitiveCase(extension, ".kmz"))
        format = "LIBKML";
    else if (compareInsensitiveCase(extension, ".json"))
        format = "GeoJSON";
    else if (compareInsensitiveCase(extension, ".osm"))
        format = "OSM";
    else format = "";
	
    return format;
}


} // End namespace internal

} // End namespace tl

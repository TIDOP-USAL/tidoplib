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

#include "tidop/viewer/group/PointCloud.h"

namespace tl
{

PointCloud::PointCloud(std::vector<Vertex> points)
    : ModelBase(std::move(points), Type::PointCloud)
{
}

PointCloud::PointCloud(std::vector<Vertex> points, const std::map<std::string, uint8_t> &attributes)
    : ModelBase(std::move(points), attributes, Type::PointCloud)
{
}

PointCloud::PointCloud(size_t _size)
    : ModelBase(_size, Type::PointCloud)
{
}

}
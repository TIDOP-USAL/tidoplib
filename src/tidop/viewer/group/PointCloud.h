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

/*! \file PointCloud.h
 *  \brief Point cloud geometric model representation.
 *
 *  This module provides a class for representing, managing, and rendering 3D point cloud
 *  data, supporting spatial coordinates, attributes, and vertex scalar fields.
 *
 *  ### Classes
 *
 *  - \ref tl::PointCloud - Renderable 3D point cloud representation subclassed from ModelBase
 *
 *  \see tl::ModelBase, tl::Vertex
 */

#pragma once

#include <iostream>
#include <vector>

#include "tidop/viewer/opengl/buffer/VertexArray.h"
#include "tidop/viewer/opengl/buffer/VertexBuffer.h"
#include "tidop/viewer/group/ModelBase.h"
#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/math/algebra/matrix/Matrix.h"


namespace tl 
{

/*! \addtogroup Geometry
 *  \{
 */

/*!
 *  \brief 3D Point Cloud entity for spatial point rendering.
 *
 *  PointCloud extends ModelBase to optimize rendering of discrete spatial points.
 *
 *  ### Usage Example
 *
 *  \code{.cpp}
 *  std::vector<tl::Vertex> points = { ... };
 *  auto cloud = PointCloud::New(std::move(points));
 *  
 *  cloud->setPointSize(2.0f);
 *  cloud->draw();
 *  \endcode
 *
 *  \see ModelBase, Vertex
 */
class PointCloud
  : public ModelBase
{

    GENERATE_SHARED_PTR(PointCloud)

public:

    /*!
     *  \brief Default constructor.
     */
    PointCloud() = default;

    /*!
     *  \brief Constructs a point cloud from a vector of vertices.
     *
     *  \param[in] points Vector of point vertices (moved into internal buffer).
     */
    PointCloud(std::vector<Vertex> points);

    /*!
     *  \brief Constructs a point cloud with custom per-vertex layout attributes.
     *
     *  \param[in] points Vector of point vertices (moved into internal buffer).
     *  \param[in] attributes Map of attribute names to location/sizes.
     */
    PointCloud(std::vector<Vertex> points,
               const std::map<std::string, uint8_t> &attributes);

    /*!
     *  \brief Pre-allocated length constructor.
     *
     *  \param[in] size Initial point capacity/length.
     */
    PointCloud(size_t _size);

    /*!
     *  \brief Default move constructor.
     */
    PointCloud(PointCloud &&) noexcept = default;

    ~PointCloud() = default;

    /*!
     *  \brief Default move assignment operator.
     */
    PointCloud &operator=(PointCloud &&) noexcept = default;
};

}
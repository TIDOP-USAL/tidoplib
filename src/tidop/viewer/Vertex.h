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

/*! \file Vertex.h
 *  \brief 3D mesh vertex representation with geometric attributes and custom scalars.
 *
 *  This module defines the Vertex structure used for rendering and spatial data processing.
 *  It stores position, color, surface normal, texture coordinates, and custom scalar values.
 *
 *  ### Classes
 *
 *  - \ref tl::Vertex - Aggregate representation of a mesh vertex
 *
 *  \see tl::Vector3f, tl::Vector4f, tl::Vector2f
 */

#pragma once

#include "tidop/math/algebra/vector/Vector.h"

namespace tl
{

/*! \addtogroup Viewer3D
 *  \{
 */

/*!
 *  \brief Representation of a 3D vertex containing geometric and material attributes.
 *
 *  Vertex holds essential data for rendering and geometric calculations, including spatial
 *  coordinates, surface normals, UV mapping, vertex colors, and optional custom scalar metrics.
 *
 *  ### Usage Example
 *
 *  \code{.cpp}
 *  // Designated initialization
 *  tl::Vertex v1{.position = {0.0f, 1.0f, 0.0f}, .color = {1.0f, 0.0f, 0.0f, 1.0f}};
 *
 *  // Construction with scalar values (using move semantics)
 *  std::vector<float> data = {0.5f, 1.2f};
 *  tl::Vertex v2({1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, std::move(data));
 *
 *  // Coordinate swapping
 *  v1.swapYZ();
 *  \end{code}
 *
 *  \see Vector3f, Vector4f, Vector2f
 */
struct Vertex
{
    Vector3f position{Vector3f::zero()};   //!< 3D position vector in spatial coordinates.
    Vector4f color{Vector4f::zero()};      //!< RGBA color component representation.
    Vector3f normal{Vector3f::zero()};     //!< Surface normal vector for lighting calculations.
    Vector2f uvs{Vector2f::zero()};        //!< 2D texture coordinates (U, V).

    std::vector<float> scalars;            //!< Optional list of custom per-vertex scalar values.

    /*!
     *  \brief Default constructor.
     *
     *  Constructs a vertex with all member components set to their default initialized values.
     */
    Vertex() = default;

    /*!
     *  \brief Constructs a vertex with position, color, and optional scalars.
     *
     *  \param[in] _position 3D spatial position vector.
     *  \param[in] _color RGBA color vector.
     *  \param[in] _scalars  Optional vector of custom scalar metrics (moved internally).
     */
    constexpr Vertex(Vector3f _position, Vector4f _color, std::vector<float> _scalars = {}) noexcept
      : position(std::move(_position)),
        color(std::move(_color)),
        scalars(std::move(_scalars))
    {
    }

    /*!
     *  \brief Constructs a fully specified geometric vertex.
     *
     *  \param[in] _position  3D spatial position vector.
     *  \param[in] _color  RGBA color vector.
     *  \param[in] _normal Surface normal vector.
     *  \param[in] _uvs   Texture UV coordinates vector.
     */
    constexpr Vertex(Vector3f _position, Vector4f _color, Vector3f _normal, Vector2f _uvs) noexcept
      : position(std::move(_position)),
        color(std::move(_color)), 
        normal(std::move(_normal)),
        uvs(std::move(_uvs)) 
    {
    }

    /*!
     *  \brief Swaps Y and Z components of the position vector and inverts the X component.
     *
     *  Useful for coordinate space transformations (e.g., converting between Y-up and Z-up conventions).
     */
    constexpr void swapYZ() noexcept
    {
        position.x() *= -1.f;
        std::swap(position.y(), position.z());
    }

    /*!
     *  \brief Compares two Vertex instances lexicographically.
     *
     *  Uses the C++20 three-way comparison operator (`<=>`) to automatically synthesize
     *  all relational operators (`==`, `!=`, `<`, `<=`, `>`, `>=`).
     */
    auto operator<=>(const Vertex &) const = default;
};

/*! \} */

} // namespace tl
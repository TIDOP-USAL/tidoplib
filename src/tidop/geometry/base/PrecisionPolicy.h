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

/*! \file PrecisionPolicy.h
 * \brief Precision policy for geometric operations.
 *
 * This file defines the `PrecisionPolicy` class which controls the precision
 * of geometric computations, either by keeping the native type (Native) or
 * using a fixed precision model (FixedPrecisionModel) that converts coordinates
 * to 64-bit integers with a given resolution.
 *
 * The precision policy is fundamental for ensuring deterministic and robust
 * results in geometric operations, especially when dealing with floating-point
 * numbers.
 *
 * ### Available modes
 * - **Native**: Works directly with the original type (float, double, int).
 * - **FixedPrecisionModel**: Converts to 64-bit integers scaled by a resolution.
 *
 * \see tl::PrecisionModel
 */

#pragma once

#include "tidop/config.h"
#include "tidop/geometry/base/Dimension.h"
#include "tidop/geometry/base/Concepts.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/LineString.h"
#include "tidop/geometry/primitives/Polygon.h"

namespace tl
{
	
/*! \addtogroup Geometry
 *  \{
 */

 /*!
  * \enum PrecisionModel
  * \brief Supported precision modes.
  */
enum class PrecisionModel
{
    Native,                 /*!< Use native type (float, double, int) without conversion. */
    FixedPrecisionModel     /*!< Convert to 64-bit integer with fixed resolution. */
};


/*!
 * \class PrecisionPolicy
 * \brief Precision policy for geometric coordinates.
 *
 * This template class defines how precision is handled in geometric operations.
 * Depending on the mode, it may snap coordinates to a fixed resolution or simply
 * keep the original value.
 *
 * \tparam Scalar Coordinate scalar type (float, double, int, etc.).
 * \tparam Mode   Precision mode (Native or FixedPrecisionModel).
 *
 * \note The specialization for FixedPrecisionModel requires a positive resolution
 *       in the constructor.
 *
 * ### Example usage
 * \code
 * using Policy = PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel>;
 * Policy pol(1e-6); // resolution of 1e-6
 * auto snapped = pol.snap(3.1415926535); // 64-bit integer
 * \endcode
 */
template<typename Scalar, PrecisionModel Mode = PrecisionModel::Native
>
class PrecisionPolicy;


/*!
 * \brief Specialization for Native mode.
 *
 * In Native mode, no conversion is performed; operations work directly with
 * the original type.
 */
template<typename Scalar>
class PrecisionPolicy<Scalar, PrecisionModel::Native>
{

public:

    using scalar_type = Scalar;
    using calc_type   = Scalar;  // mismo tipo
    using KernelPoint2D = Point<calc_type, xy_tag>;
    using KernelPoint3D = Point<calc_type, xyz_tag>;

    static constexpr PrecisionModel mode = PrecisionModel::Native;

    /*!
     * \brief Default constructor.
     */
    constexpr PrecisionPolicy() = default;

    /*!
     * \brief Apply the precision policy to a value.
     *
     * In Native mode, returns the value unchanged.
     *
     * \param[in] v Input value.
     * \return The same value \p v.
     */
    constexpr auto snap(Scalar v) const noexcept -> calc_type
    {
        return v;
    }

    template<Dimension Dim, typename Point_t>
    constexpr auto toKernelPoint(const Point_t &p) const noexcept
    {
        if constexpr (Dim == Dimension::dim2) {
            return to_kernel_point_2d(p);
        } else if constexpr (Dim == Dimension::dim3) {
            return to_kernel_point_3d(p);
        }
    }

    template<Dimension Dim, LinearGeometryConcept LS>
    constexpr auto toKernelLineString(const LS &lineString) const noexcept
    {
        using KP = Point<calc_type, typename tag_for_dim<dimension_value(Dim)>::type>;

        LineString<KP> result;
        result.reserve(lineString.size());

        for (const auto &point : lineString)
            result.push_back(toKernelPoint<Dim>(point));

        return result;
    }

    template<Dimension Dim, PolygonConcept Polygon>
    constexpr auto toKernelPolygon(const Polygon &polygon) const noexcept
    {
        using KP = Point<calc_type, typename tag_for_dim<dimension_value(Dim)>::type>;

        tl::Polygon<KP> result;

        for (const auto &point : polygon.outer())
            result.outer().push_back(toKernelPoint<Dim>(point));

        for (const auto &hole : polygon.inners()) {
            auto &outHole = result.inners().emplace_back();
            for (const auto &p : hole)
                outHole.push_back(toKernelPoint<Dim>(p));
        }

        return result;
    }

private:

    template<typename Point_t>
    static auto to_kernel_point_2d(const Point_t &p) -> Point<calc_type, xy_tag>
    {
        return static_cast<Point<calc_type, xy_tag>>(p);
    }

    template<typename Point_t>
    static auto to_kernel_point_3d(const Point_t &p) -> Point<calc_type, xyz_tag>
    {
        return static_cast<Point<calc_type, xyz_tag>>(p);
    }
};


/*!
 * \brief Specialization for FixedPrecisionModel mode.
 *
 * In this mode, coordinates are converted to 64-bit integers using a fixed
 * resolution. This enables robust and deterministic geometric operations.
 */
template<typename Scalar>
class PrecisionPolicy<Scalar, PrecisionModel::FixedPrecisionModel>
{

private:

    double mScale;

public:

    using scalar_type = Scalar;
    using calc_type   = std::int64_t;
    using KernelPoint2D = Point<calc_type, xy_tag>;
    using KernelPoint3D = Point<calc_type, xyz_tag>;

    static constexpr PrecisionModel mode = PrecisionModel::FixedPrecisionModel;

    /*!
     * \brief Constructor with resolution.
     *
     * \param[in] resolution Desired resolution (minimum representable distance).
     *                       Must be a positive value.
     *
     * \pre resolution > 0
     */
    constexpr explicit PrecisionPolicy(double resolution)
    {
        if (resolution <= 0.) {
            mScale = 1.0;
        } else if (resolution < 1e-15) {
            mScale = 1e15; // evitar overflow
        } else {
            mScale = 1.0 / resolution;
        }
    }

    /*!
     * \brief Apply the precision policy to a value.
     *
     * Converts the scalar value to a 64-bit integer by rounding to the nearest
     * integer after multiplying by the scale factor.
     *
     * \param[in] v Input value.
     * \return Integer value representing \p v at the fixed resolution.
     */
    constexpr auto snap(Scalar v) const noexcept -> calc_type
    {
        if constexpr (std::is_integral_v<Scalar>) {
            return static_cast<calc_type>(v);
        } else {
            return std::llround(v * mScale);
        }
    }

    template<Dimension Dim, PointConcept Point>
    constexpr auto toKernelPoint(const Point &point) const noexcept
    {
        if constexpr (Dim == Dimension::dim2) {
            return toKernelPoint2d(point);
        } else if constexpr (Dim == Dimension::dim3) {
            return toKernelPoint3d(point);
        }
    }

    template<Dimension Dim, LinearGeometryConcept LS>
    constexpr auto toKernelLineString(const LS &lineString) const noexcept
    {
        using KP = Point<calc_type, typename tag_for_dim<dimension_value(Dim)>::type>;

        LineString<KP> result;
        result.reserve(lineString.size());

        for (const auto &point : lineString)
            result.push_back(toKernelPoint<Dim>(point));

        return result;
    }

    template<Dimension Dim, PolygonConcept Polygon>
    constexpr auto toKernelPolygon(const Polygon &polygon) const noexcept
    {
        using KP = Point<calc_type, typename tag_for_dim<dimension_value(Dim)>::type>;

        tl::Polygon<KP> result;

        for (const auto &point : polygon.outer())
            result.outer().push_back(toKernelPoint<Dim>(point));

        for (const auto &hole : polygon.inners()) {
            auto &outHole = result.inners().emplace_back();
            for (const auto &p : hole)
                outHole.push_back(toKernelPoint<Dim>(p));
        }

        return result;
    }

private:

    template<PointConcept P>
    auto toKernelPoint2d(const P &point) const -> KernelPoint2D
    {
        return KernelPoint2D(snap(point.x()),
                             snap(point.y()));
    }

    template<PointConcept P>
    auto toKernelPoint3d(const P &point) const -> KernelPoint3D
    {
        return KernelPoint3D(snap(point.x()),
                             snap(point.y()),
                             snap(point.z()));
    }

};



/*! \} */ 

} // End namespace tl

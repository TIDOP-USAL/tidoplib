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

#pragma once

#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/Multipolygon.h"
#include "tidop/graphic/entities/GraphicEntity.h"

namespace tl
{

class Painter;

/*! \addtogroup GraphicEntities
 *  \{
 */


/*!
 * \brief Multi-polygon 3D graphic class
 */
class TL_EXPORT GMultiPolygon3D
  : public GraphicEntity
{

private:

    MultiPolygon<Point3d> mGeometry;

public:

    /*!
     * \brief Default constructor
     */
    GMultiPolygon3D() = default;

    /*!
     * \brief Constructor from a MultiPolygon
     * \param multiPolygon MultiPolygon object
     * \see MultiPolygon
     */
    explicit GMultiPolygon3D(const MultiPolygon<Point3d> &multiPolygon);

    explicit GMultiPolygon3D(size_t size);

    /*!
     * \brief Copy constructor
     * \param multiPolygon3D Object to be copied
     */
    GMultiPolygon3D(const GMultiPolygon3D &multiPolygon3D) = default;

    /*!
     * \brief Move Constructor
     * \param[in] multiPolygon3D GPolygon object that moves
     */
    GMultiPolygon3D(GMultiPolygon3D &&multiPolygon3D) noexcept = default;

    ~GMultiPolygon3D() override = default;

    /*!
     * \brief Operador de asignación
     * \param multiPolygon3D Objeto GMultiPolygon3D que se copia
     * \return Referencia al objeto
     */
    auto operator =(const GMultiPolygon3D &multiPolygon3D) -> GMultiPolygon3D& = default;

    /*!
     * \brief Move assignment operator
     * \param[in] multiPolygon3D GMultiPolygon3D object that moves
     * \return Object reference
     */
    auto operator =(GMultiPolygon3D &&multiPolygon3D) noexcept -> GMultiPolygon3D & = default;

    auto geometry() const -> const MultiPolygon<Point3d> & { return mGeometry; }
    auto geometry() -> MultiPolygon<Point3d> & { return mGeometry; }

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
    auto window() const -> BoundingBox<Point2d> override;
    void draw(Painter &painter) const override;
    auto clone() const -> std::unique_ptr<GraphicEntity> override;
};



inline auto GMultiPolygon3D::isMultiEntity() const -> bool
{
    return true;
}

inline auto GMultiPolygon3D::isSimpleEntity() const -> bool
{
    return false;
}


/*! \} */ // Fin GraphicEntities

} // namespace tl

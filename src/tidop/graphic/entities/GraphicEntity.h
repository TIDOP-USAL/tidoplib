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

#include <string>
#include <vector>
#include <array>
#include <memory>

#include "tidop/core/base/Defs.h"
#include "tidop/core/base/Meta.h"
#include "tidop/graphic/Color.h"
#include "tidop/graphic/Styles.h"
#include "tidop/graphic/model/TableRegister.h"
#include "tidop/geometry/spatial/BoundingBox.h"
#include "tidop/geometry/primitives/Point.h"

namespace tl
{

class Painter;

/*! \addtogroup GraphicEntities
 *  \{
 */

/*!
 * \class GraphicEntity
 * \brief Base class for drawable graphical entities.
 *
 * A `GraphicEntity` represents a visual geometry that can be rendered using a `Painter`.
 * It inherits from `GraphicStyle`, meaning each entity also carries its own visual style
 * (pen, brush, symbol, label).
 *
 * This class stores the entity's geometry type and an optional pointer to attribute data
 * (`TableRegister`), and defines a uniform interface for drawing and classification
 * (e.g., is multi-entity or not).
 *
 * Derived classes are expected to implement specific geometric behavior and rendering logic.
 *
 * \see GraphicStyle, Painter, TableRegister
 */
class TL_EXPORT GraphicEntity
{

public:

    /*!
     * \enum Type
     * \brief Enumeration of supported geometric entity types.
     *
     * This enum defines the different geometry types that a `GraphicEntity` can represent,
     * such as 2D/3D points, lines, polygons, and shapes.
     */
    enum class Type
    {
        point_2d,                   /*!< Point 2D */
        point_3d,                   /*!< 3D Point */
        linestring_2d,              /*!< 2D Polyline */
        linestring_3d,              /*!< 3D Polyline */
        polygon_2d,                 /*!< 2D Polygon */
        polygon_3d,                 /*!< 3D Polygon */
        segment_2d,                 /*!< 2D Segment */
        segment_3d,                 /*!< 3D Segment */
        window,                     /*!< Window */
        box,                        /*!< Box */
        multipoint_2d,              /*!< Multipoint 2D */
        multipoint_3d,              /*!< Multi-point 3D */
        multiline_2d,               /*!< Multi-line 2D */
        multiline_3d,               /*!< Multi-line 3D */
        multipolygon_2d,            /*!< Multi-polygon 2D */
        multipolygon_3d,            /*!< Multi-polygon 3D */
        circle,                     /*!< Circle */
        ellipse                     /*!< Ellipse */
    };

protected:

    Type mEntityType;
    TableRegister mData;
    GraphicStyle mStyle;

public:

    GraphicEntity() = default;

    /*!
     * \brief Constructs a graphical entity of the given type.
     * \param[in] type Geometry type of the entity.
     */
    explicit GraphicEntity(Type type, const GraphicStyle &style = {});

    /*!
     * \brief Copy constructor.
     * \param[in] graphicEntity Entity to copy.
     */
    GraphicEntity(const GraphicEntity &graphicEntity) = default;

    /*!
     * \brief Move constructor.
     * \param[in] graphicEntity Entity to move.
     */
    GraphicEntity(GraphicEntity &&graphicEntity) noexcept = default;

    virtual ~GraphicEntity() = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] graphicEntity Entity to copy.
     * \return Reference to this object.
     */
    auto operator =(const GraphicEntity& graphicEntity) -> GraphicEntity& = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] graphicEntity Entity to move.
     * \return Reference to this object.
     */
    auto operator =(GraphicEntity&& graphicEntity) noexcept -> GraphicEntity& = default;

    void setStyle(const GraphicStyle &style) { mStyle = style; }
    [[nodiscard]]
    auto style() const -> const GraphicStyle & { return mStyle; }   

    void setPen(const Pen &pen) { mStyle.setPen(pen); }
    [[nodiscard]]
    auto pen() const -> const Pen * { return mStyle.pen(); }
    void setBrush(const Brush &brush) { mStyle.setBrush(brush); }
    [[nodiscard]] 
    auto brush() const -> const Brush * { return mStyle.brush(); }
    void setSymbol(const Symbol &symbol) { mStyle.setSymbol(symbol); }
    [[nodiscard]] 
    auto symbol() const -> const Symbol * { return mStyle.symbol(); }
    void setLabel(const Label &label) { mStyle.setLabel(label); }
    [[nodiscard]] 
    auto label() const -> const Label * { return mStyle.label(); }

    /*!
     * \brief Returns the geometric type of the entity.
     * \return Type of the entity.
     */
    auto type() const -> Type;

    /*!
     * \brief Checks whether the entity is a multi-geometry.
     * \return True if the entity is a multi-geometry type.
     */
    virtual auto isMultiEntity() const -> bool = 0;

    /*!
     * \brief Checks whether the entity is a simple (non-multi) geometry.
     * \return True if the entity is a simple geometry type.
     */
    virtual auto isSimpleEntity() const -> bool = 0;

    /*!
     * \brief Renders the entity using the given painter.
     * \param[in] painter Painter used to draw the entity.
     */
    virtual void draw(Painter &painter) const = 0;

    /*!
     * \brief Returns the 2D bounding window of the entity.
     * \return Bounding window of the entity.
     */
    virtual auto window() const -> BoundingBox<Point2d> = 0;

    /*!
     * \brief Returns the attribute data associated with this entity.
     * \return Shared pointer to the attribute table entry.
     */
    auto attributes() const -> TableRegister;

    /*!
     * \brief Associates attribute data with this entity.
     * \param[in] data Pointer to the attribute table entry.
     */
    void setAttributes(const TableRegister &attributes);

    virtual auto clone() const -> std::unique_ptr<GraphicEntity> = 0;

};


/*! \} */ // End GraphicEntities

} // namespace tl

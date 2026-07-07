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

/*! \file GraphicEntity.h
 * \brief Base class for drawable graphical entities.
 *
 * This file defines the `GraphicEntity` class, which serves as the abstract base
 * for all renderable geometric objects. Each entity has a geometry type, a visual
 * style (pen, brush, symbol, label), and optional attribute data. Derived classes
 * must implement the rendering logic (`draw`), bounding box (`window`), and cloning.
 *
 * \ingroup Graphics
 * \see tl::GraphicStyle, tl::Painter, tl::TableRegister
 */

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

    Type mEntityType = Type::point_2d;
    TableRegister mData;
    GraphicStyle mStyle;

public:

    GraphicEntity() = default;

    /*!
     * \brief Constructs a graphical entity of the given type.
     * \param[in] type  Geometry type of the entity.
     * \param[in] style Initial visual style (default: empty).
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

    /*!
     * \brief Sets the visual style of the entity.
     * \param[in] style The new style.
     */
    void setStyle(const GraphicStyle &style) { mStyle = style; }
    [[nodiscard]]

    /*!
     * \brief Returns the current visual style.
     * \return Const reference to the style.
     */
    auto style() const -> const GraphicStyle & { return mStyle; }   

    /*!
     * \brief Sets the pen (stroke) of the entity.
     * \param[in] pen The new pen.
     */
    void setPen(const Pen &pen) { mStyle.setPen(pen); }

    /*!
     * \brief Returns the current pen.
     * \return Pointer to the pen, or `nullptr` if not set.
     */
    [[nodiscard]]
    auto pen() const -> const Pen * { return mStyle.pen(); }

    /*!
     * \brief Sets the brush (fill) of the entity.
     * \param[in] brush The new brush.
     */
    void setBrush(const Brush &brush) { mStyle.setBrush(brush); }

    /*!
     * \brief Returns the current brush.
     * \return Pointer to the brush, or `nullptr` if not set.
     */
    [[nodiscard]] 
    auto brush() const -> const Brush * { return mStyle.brush(); }

    /*!
     * \brief Sets the symbol (marker) of the entity.
     * \param[in] symbol The new symbol.
     */
    void setSymbol(const Symbol &symbol) { mStyle.setSymbol(symbol); }

    /*!
     * \brief Returns the current symbol.
     * \return Pointer to the symbol, or `nullptr` if not set.
     */
    [[nodiscard]] 
    auto symbol() const -> const Symbol * { return mStyle.symbol(); }

    /*!
     * \brief Sets the label of the entity.
     * \param[in] label The new label.
     */
    void setLabel(const Label &label) { mStyle.setLabel(label); }

    /*!
     * \brief Returns the current label.
     * \return Pointer to the label, or `nullptr` if not set.
     */
    [[nodiscard]] 
    auto label() const -> const Label * { return mStyle.label(); }

    /*!
     * \brief Returns the geometric type of the entity.
     * \return Type of the entity.
     */
    auto type() const -> Type;

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
     * \return A `TableRegister` containing the attribute data.
     */
    auto attributes() const -> TableRegister;

    /*!
     * \brief Associates attribute data with this entity.
     * \param[in] data The attribute data to store.
     */
    void setAttributes(const TableRegister &attributes);

    /*!
     * \brief Creates a deep copy of this entity.
     * \return A unique pointer to the cloned entity.
     */
    virtual auto clone() const -> std::unique_ptr<GraphicEntity> = 0;

};


/*! \} */

} // namespace tl

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

/*! \file GLayer.h
 * \brief Layer container for graphic entities.
 *
 * This file defines the `GLayer` class, which represents a collection of
 * `GraphicEntity` objects (points, polylines, polygons, etc.) organized as a
 * named layer. It provides methods for managing entities, accessing attribute
 * schema, and rendering the entire layer using a `Painter`.
 *
 * \ingroup Graphics
 * \see tl::GraphicEntity, tl::Painter, tl::TableField
 */

#pragma once

#include <vector>
#include <memory>
#include <string>

#include "tidop/config.h"
#include "tidop/graphic/model/TableField.h"
#include "tidop/geometry/spatial/BoundingBox.h"

namespace tl
{

class Painter;

/*! \addtogroup Graphic
 *  \{
 */

class GraphicEntity;

/*!
 * \class GLayer
 * \brief Named layer containing a collection of graphic entities.
 *
 * A `GLayer` stores a list of `GraphicEntity` objects (owned via `std::unique_ptr`),
 * along with a name and an attribute schema (list of `TableField`). It provides
 * STL-like iteration, insertion, removal, and rendering capabilities.
 *
 * The layer does not copy entities; it moves them into its internal storage.
 * Copy construction and assignment are supported but perform deep copies of
 * all entities (via `clone()`).
 *
 * ### Example
 * \code
 * GLayer layer("MyLayer");
 * auto point = std::make_unique<GPoint>(10, 20);
 * layer.push_back(std::move(point));
 * layer.draw(painter);
 * \endcode
 */
class TL_EXPORT GLayer
{

public:

    using value_type = std::vector<std::unique_ptr<GraphicEntity> >::value_type;
    using iterator = std::vector<std::unique_ptr<GraphicEntity> >::iterator;
    using const_iterator = std::vector<std::unique_ptr<GraphicEntity> >::const_iterator;

protected:

    std::string mName;
    std::vector<std::unique_ptr<GraphicEntity>> mEntities;
    GraphicEntity *mSelectEntity;
    std::vector<TableField> mTableFields;

public:

    /*!
     * \brief Default constructor.
     * Creates an empty layer with an empty name.
     */
    GLayer();

    /*!
     * \brief Copy constructor.
     * Performs a deep copy of all entities in the source layer.
     * \param[in] gLayer Layer to copy.
     */
    GLayer(const GLayer &gLayer);

    /*!
     * \brief Move constructor.
     * Transfers ownership of entities from the source layer.
     * \param[in] gLayer Layer to move.
     */
    GLayer(GLayer &&gLayer) noexcept;

    /*! \brief Destructor. */
    ~GLayer() = default;

    /*!
     * \brief Returns the name of the layer.
     * \return Layer name.
     */
    auto name() const -> std::string;

    /*!
     * \brief Sets the name of the layer.
     * \param[in] name New layer name.
     */
    void setName(const std::string &name);

    /*!
     * \brief Returns an iterator to the beginning of the entity list.
     */
    auto begin() noexcept -> iterator;

    /*!
     * \brief Returns a constant iterator to the beginning of the entity list.
     */
    auto begin() const noexcept -> const_iterator;

    /*!
     * \brief Returns an iterator to the end of the entity list.
     */
    auto end() noexcept -> iterator;

    /*!
     * \brief Returns a constant iterator to the end of the entity list.
     */
    auto end() const noexcept -> const_iterator;

    /*!
     * \brief Appends a new entity to the layer (takes ownership).
     * \param[in] entity Unique pointer to the entity (moved into the layer).
     */
    void push_back(std::unique_ptr<GraphicEntity> entity);

    /*!
     * \brief Removes all entities from the layer.
     */
    void clear() noexcept;

    /*!
     * \brief Checks whether the layer is empty.
     * \return `true` if the entity list is empty; `false` otherwise.
     */
    auto empty() const noexcept -> bool;

    /*!
     * \brief Resizes the entity container.
     * \param[in] count New container size.
     * If `count` is larger than the current size, default-constructed
     * (empty) `unique_ptr<GraphicEntity>` are added.
     */
    void resize(size_t count);

    /*!
     * \brief Returns the number of entities in the layer.
     * \return Entity count.
     */
    auto size() const noexcept -> size_t;

    /*!
     * \brief Erases a range of entities.
     * \param[in] first Iterator to the beginning of the range.
     * \param[in] last Iterator to the end of the range.
     * \return Iterator following the last removed element.
     */
    auto erase(const_iterator first, const_iterator last) -> iterator;

    /*!
     * \brief Copy assignment operator.
     * Performs a deep copy of all entities from the source.
     * \param[in] entity Source layer.
     * \return Reference to this layer.
     */
    auto operator=(const GLayer &entity) -> GLayer &;

    /*!
     * \brief Move assignment operator.
     * Transfers ownership of entities from the source.
     * \param[in] entity Source layer.
     * \return Reference to this layer.
     */
    auto operator=(GLayer &&entity) noexcept -> GLayer &;

    /*!
     * \brief Adds a table field (attribute definition) to the layer's schema.
     * \param[in] field The field definition.
     */
    void addDataField(TableField field);

    /*!
     * \brief Returns the list of table fields (attribute schema).
     * \return Vector of field definitions.
     */
    auto tableFields() const -> const std::vector<TableField> &;

    /*!
     * \brief Renders all entities in the layer using the provided painter.
     * \param[in] painter Painter object used for rendering.
     */
    void draw(Painter &painter) const;

    /*!
     * \brief Computes the bounding box that encloses all entities.
     * \return A `BoundingBox<Point2d>` covering the entire layer.
     * \note If the layer is empty, returns an empty (invalid) bounding box.
     */
    auto boundingBox() const -> BoundingBox<Point2d>;
};


/*! \} */ //  GraphicEntities

} // namespace tl

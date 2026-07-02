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
 * \brief Represents a layer that holds a collection of graphical entities.
 *
 * A `GLayer` contains a list of `GraphicEntity` objects that can be rendered
 * together as a logical group. Layers may hold entities of one or multiple types
 * (e.g., points, lines, polygons), and can also store optional table fields associated
 * with their attributes.
 *
 * Layers support basic container operations such as iteration, insertion, resizing,
 * and drawing.
 *
 * \see GraphicEntity, Painter, TableField
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
     * \brief Default constructor
     */
    GLayer();

    /*!
     * \brief Copy constructor.
     * \param[in] gLayer Layer to copy.
     */
    GLayer(const GLayer &gLayer);

    /*!
     * \brief Move constructor.
     * \param[in] gLayer Layer to move.
     */
    GLayer(GLayer &&gLayer) noexcept;

    ~GLayer() = default;

    /*!
     * \brief Returns the name of the layer.
     * \return Layer name.
     */
    auto name() const -> std::string;

    /*!
     * \brief Sets the name of the layer.
     * \param[in] name Layer name.
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
     * \brief Appends a new entity to the layer.
     * \param[in] entity Shared pointer to the entity.
     */
    void push_back(std::unique_ptr<GraphicEntity> entity);

    /*!
     * \brief Removes all entities from the layer.
     */
    void clear() noexcept;

    /*!
     * \brief Checks whether the layer is empty.
     * \return True if the entity list is empty.
     */
    auto empty() const noexcept -> bool;

    /*!
     * \brief Resizes the entity container.
     * If the current size is less than count, additional elements are added. If the current size
     * is greater than count, the container is truncated to the specified number of elements.
     * \param[in] count New container size
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
     * \brief Copy assignment
     */
    auto operator=(const GLayer &entity) -> GLayer &;

    /*!
     * \brief Move assignment
     */
    auto operator=(GLayer &&entity) noexcept -> GLayer &;

    /*!
     * \brief Adds a table field (attribute definition) to the layer.
     * \param[in] field pointer to the field.
     */
    void addDataField(TableField field);

    /*!
     * \brief Returns the list of table fields (attribute schema).
     * \return Vector of field definitions.
     */
    auto tableFields() const -> const std::vector<TableField>&;

    /*!
     * \brief Draws all entities in the layer using the provided painter.
     * \param[in] painter Painter object used for rendering.
     */
    void draw(Painter &painter) const;

    /*!
     * \brief Returns the bounding box (window) that encloses all entities.
     * \return Bounding window of the layer contents.
     */
    auto boundingBox() const -> BoundingBox<Point2d>;

    //TL_DEPRECATED("Use boundingBox() instead")
    auto window() const -> BoundingBox<Point2d>;
};


/*! \} */ //  GraphicEntities

} // namespace tl

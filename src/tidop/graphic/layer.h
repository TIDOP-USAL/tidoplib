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


#include <list>
#include <memory>
#include <string>

#include "tidop/core/base/defs.h"
#include "tidop/graphic/datamodel.h"
#include "tidop/geometry/window.h"

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

    using allocator_type = std::list<std::shared_ptr<GraphicEntity> >::allocator_type;
    using value_type = std::list<std::shared_ptr<GraphicEntity> >::value_type;
    using size_type = std::list<std::shared_ptr<GraphicEntity> >::size_type;
    using difference_type = std::list<std::shared_ptr<GraphicEntity> >::difference_type;
    using pointer = std::list<std::shared_ptr<GraphicEntity> >::pointer;
    using const_pointer = std::list<std::shared_ptr<GraphicEntity> >::const_pointer;
    using reference = std::list<std::shared_ptr<GraphicEntity> >::reference;
    using const_reference = std::list<std::shared_ptr<GraphicEntity> >::const_reference;
    using iterator = std::list<std::shared_ptr<GraphicEntity> >::iterator;
    using const_iterator = std::list<std::shared_ptr<GraphicEntity> >::const_iterator;

protected:

    std::string mName;
    std::list<std::shared_ptr<GraphicEntity>> mEntities;
    std::shared_ptr<GraphicEntity> mSelectEntity;
    std::vector<std::shared_ptr<TableField>> mTableFields;

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
    GLayer(GLayer &&gLayer) TL_NOEXCEPT;

    /*!
     * \brief Constructs the layer with a list of entities.
     * \param[in] entities Initial list of graphical entities.
     */
    GLayer(std::initializer_list<std::shared_ptr<GraphicEntity>> entities);

    ~GLayer() = default;

    /*!
     * \brief Returns the name of the layer.
     * \return Layer name.
     */
    auto name() const->std::string;

    /*!
     * \brief Sets the name of the layer.
     * \param[in] name Layer name.
     */
    void setName(const std::string &name);

    /*!
     * \brief Returns an iterator to the beginning of the entity list.
     */
    auto begin() TL_NOEXCEPT -> iterator;

    /*!
     * \brief Returns a constant iterator to the beginning of the entity list.
     */
    auto begin() const TL_NOEXCEPT -> const_iterator;

    /*!
     * \brief Returns an iterator to the end of the entity list.
     */
    auto end() TL_NOEXCEPT -> iterator;

    /*!
     * \brief Returns a constant iterator to the end of the entity list.
     */
    auto end() const TL_NOEXCEPT -> const_iterator;

    /*!
     * \brief Appends a new entity to the layer.
     * \param[in] entity Shared pointer to the entity.
     */
    void push_back(const std::shared_ptr<GraphicEntity> &entity);

    /*!
     * \brief Appends a new entity using move semantics.
     * \param[in] entity Rvalue reference to the entity.
     */
    void push_back(std::shared_ptr<GraphicEntity> &&entity) TL_NOEXCEPT;

    /*!
     * \brief Removes all entities from the layer.
     */
    void clear() TL_NOEXCEPT;

    /*!
     * \brief Checks whether the layer is empty.
     * \return True if the entity list is empty.
     */
    auto empty() const TL_NOEXCEPT -> bool;

    /*!
     * \brief Resizes the entity container.
     * If the current size is less than count, additional elements are added. If the current size
     * is greater than count, the container is truncated to the specified number of elements.
     * \param[in] count New container size
     */
    void resize(size_type count);

    /*!
     * \brief Resizes the container and initializes new elements with the given value.
     * If the current size is less than count, additional elements are added and initialized with value.
     * If the current size is greater than count, the container is truncated to the specified number of elements.
     * \param[in] count New container size
     * \param[in] value Value assigned to the new elements
     */
    void resize(size_type count,
        const std::shared_ptr<GraphicEntity> &value);

    /*!
     * \brief Returns the number of entities in the layer.
     * \return Entity count.
     */
    auto size() const TL_NOEXCEPT -> size_type;

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
    auto operator=(GLayer &&entity) TL_NOEXCEPT -> GLayer &;

    /*!
     * \brief Adds a table field (attribute definition) to the layer.
     * \param[in] field Shared pointer to the field.
     */
    void addDataField(const std::shared_ptr<TableField> &field);

    /*!
     * \brief Returns the list of table fields (attribute schema).
     * \return Vector of field definitions.
     */
    auto tableFields() const -> std::vector<std::shared_ptr<TableField>>;

    /*!
     * \brief Draws all entities in the layer using the provided painter.
     * \param[in] painter Painter object used for rendering.
     */
    void draw(Painter &painter) const;

    /*!
     * \brief Returns the bounding box (window) that encloses all entities.
     * \return Bounding window of the layer contents.
     */
    auto window() const -> Window<Point<double>>;

};


/*! \} */ //  GraphicEntities

} // namespace tl

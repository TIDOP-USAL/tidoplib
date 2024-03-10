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

#include "tidop/core/defs.h"
#include "tidop/graphic/datamodel.h"
#include "tidop/geometry/entities/window.h"

namespace tl
{


/*! \addtogroup GraphicEntities
 *  \{
 */

class GraphicEntity;

/*!
 * Layer. Can contain elements of one or several types. They can
 * be graphical entities or simple ones.
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
     * \brief Copy constructor
     * \param[in] gLayer Object being copied
     */
    GLayer(const GLayer &gLayer);

    /*!
     * \brief Move constructor
     * \param[in] gLayer Object being moved
     */
    GLayer(GLayer &&gLayer) TL_NOEXCEPT;

    /*!
     * \brief List constructor
     * \param[in] entities List of entities
     */
    GLayer(std::initializer_list<std::shared_ptr<GraphicEntity>> entities);

    ~GLayer() = default;

    auto begin() TL_NOEXCEPT -> iterator;
    auto begin() const TL_NOEXCEPT -> const_iterator;
    auto end() TL_NOEXCEPT -> iterator;
    auto end() const TL_NOEXCEPT -> const_iterator;

    void push_back(const std::shared_ptr<GraphicEntity> &entity);
    void push_back(std::shared_ptr<GraphicEntity> &&entity) TL_NOEXCEPT;

    /*!
     * \brief Clears the container
     */
    void clear() TL_NOEXCEPT;

    /*!
     * \brief Checks if the container is empty
     * \return true if the container is empty and false otherwise
     */
    auto empty() const TL_NOEXCEPT -> bool;

    /*!
     * \brief Modifies the container size
     * If the current size is less than count, additional elements are added. If the current size
     * is greater than count, the container is truncated to the specified number of elements.
     * \param[in] count New container size
     */
    void resize(size_type count);

    /*!
     * \brief Modifies the container size
     * If the current size is less than count, additional elements are added and initialized with value.
     * If the current size is greater than count, the container is truncated to the specified number of elements.
     * \param[in] count New container size
     * \param[in] value Value assigned to the new elements
     */
    void resize(size_type count,
        const std::shared_ptr<GraphicEntity> &value);

    /*!
     * \brief Returns the container size
     * \return Size
     */
    auto size() const TL_NOEXCEPT -> size_type;

    /*!
     * \brief Copy assignment
     */
    auto operator=(const GLayer &entity) -> GLayer &;

    /*!
     * \brief Move assignment
     */
    auto operator=(GLayer &&entity) TL_NOEXCEPT -> GLayer &;

    /*!
     * \brief Erases the range
     */
    auto erase(const_iterator first, const_iterator last) -> iterator;

    /*!
     * \brief Returns the layer name
     * \return Layer name
     */
    auto name() const->std::string;

    /*!
     * \brief Sets the layer name
     * \param[in] name Layer name
     */
    void setName(const std::string &name);

    void addDataField(const std::shared_ptr<TableField> &field);

    auto tableFields() const -> std::vector<std::shared_ptr<TableField>>;

    auto window() const -> Window<Point<double>>;

};


/*! \} */ //  GraphicEntities

} // namespace tl

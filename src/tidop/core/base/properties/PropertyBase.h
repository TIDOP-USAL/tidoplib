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

/*! \file PropertyBase.h
 * \brief Abstract base class for typed properties.
 *
 * This file defines the `PropertyBase` class, which provides a common interface
 * for properties that can be converted to and from strings. It is intended to
 * be subclassed by concrete property types (e.g., `Property<int>`, `Property<double>`).
 *
 * \ingroup Core
 * \see tl::Property
 */

#pragma once

#include "tidop/config.h"

#include <string>
#include <memory>

#include "tidop/core/base/Type.h"

namespace tl
{

/*! \addtogroup Base
 *  \{
 */

/*!
 * \class PropertyBase
 * \brief Abstract base class for all property types.
 *
 * This class provides a common interface for properties, allowing them to be
 * converted to and from strings. It also provides type information via
 * `typeName()`. The class is designed to be used polymorphically, with concrete
 * implementations storing values of specific types.
 *
 * \see Property
 */
class PropertyBase
{

public:

    /*!
     * \brief Default constructor.
     */
    PropertyBase() = default;

    /*! \brief Virtual destructor. */
    virtual ~PropertyBase() = default;

    /*!
     * \brief Converts the property's value to a string.
     * \return A string representation of the property's value.
     */
    virtual auto toString() const -> std::string = 0;

    /*!
     * \brief Sets the property's value from a string.
     * \param[in] value A string representation of the value to set.
     * \exception Exception If the string cannot be converted to the property's type.
     */
    virtual void fromString(const std::string &value) = 0;

    /*!
     * \brief Returns the name of the property's type.
     * \return The name of the property's type as a string (e.g., `"int"`, `"double"`).
     */
	virtual auto typeName() const -> std::string = 0;

    /*!
     * \brief Creates a deep copy of this property.
     * \return A `std::unique_ptr` to the cloned property.
     */
    virtual auto clone() const -> std::unique_ptr<PropertyBase> = 0;
};

/*! \} */

} // namespace tl

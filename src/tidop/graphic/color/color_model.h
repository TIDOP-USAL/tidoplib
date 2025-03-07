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

#include "tidop/core/base/defs.h"
#include "tidop/graphic/color/color.h"

namespace tl
{

/*! \addtogroup Color
 *  \{
 */


/*!
 * \brief Interface for a color model.
 */
class TL_EXPORT ColorModel
{

public:

    /*!
     * \brief Default constructor
     */
    ColorModel();

    /*!
     * \brief Destructor
     */
    virtual ~ColorModel();

    /*!
     * \brief Converts the model to a Color object.
     * \return Color object representation
     */
    virtual auto toColor() const -> Color = 0;

    /*!
     * \brief Initializes the model from a Color object.
     * \param[in] color Color object to initialize from
     */
    virtual void fromColor(const Color &color) = 0;
};


/*! \} */

} // End namespace tl

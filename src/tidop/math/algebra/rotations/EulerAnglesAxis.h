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

namespace tl
{

/*! \addtogroup Rotations
 *  \{
 */

 /*!
  * \brief Enum representing different rotation conventions for Euler angles.
  */
enum Axes
{
    // Euler angles
    zxz, /*!< Rotation around the Z axis, then X, then Z again. */
    xyx, /*!< Rotation around the X axis, then Y, then X again. */
    yzy, /*!< Rotation around the Y axis, then Z, then Y again. */
    zyz, /*!< Rotation around the Z axis, then Y, then Z again. */
    xzx, /*!< Rotation around the X axis, then Z, then X again. */
    yxy, /*!< Rotation around the Y axis, then X, then Y again. */

    // Tait-Bryan angles
    xyz, /*!< Rotation around X, then Y, then Z. */
    yzx, /*!< Rotation around Y, then Z, then X. */
    zxy, /*!< Rotation around Z, then X, then Y. */
    xzy, /*!< Rotation around X, then Z, then Y. */
    zyx, /*!< Rotation around Z, then Y, then X. */
    yxz  /*!< Rotation around Y, then X, then Z. */
};

/*! \} */

} // End namespace tl


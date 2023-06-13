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

#include "tidop/core/defs.h"

namespace tl
{

/*! \addtogroup math
 *  \{
 */


 /*! \addtogroup algebra
  *  \{
  */

  /*! \defgroup orientation Orientation
   *  \{
   */



   /*!
    * \brief Orientation Interface
    */
class TL_EXPORT Orientation
{

public:

    /*!
     * \brief Rotacion types
     */
    enum class Type
    {
        axis_angle,      /*!< Axial-angular */
        euler_angles,    /*!< Euler/TaitBryan angles*/
        rotation_matrix, /*!< Rotation matriz */
        quaternion       /*!< Quaternions */
    };

public:

    Orientation() = default;
    virtual ~Orientation() = default;

    /*!
     * \brief Rotation type
     * \see Rotation::Type
     */
    virtual Type type() const = 0;

};


/*!
 * \brief Base class for rotations
 */
template<typename T>
class OrientationBase
    : public Orientation
{
private:

    Type rotationType;

public:

    OrientationBase(Type type);
    OrientationBase(const OrientationBase &rotation);
    ~OrientationBase() override = default;

    Type type() const override;

};



/* OrientationBase implementation */


template<typename T> inline
OrientationBase<T>::OrientationBase(Type type)
  : rotationType(type)
{
}

template<typename T> inline
OrientationBase<T>::OrientationBase(const OrientationBase &rotation)
  : rotationType(rotation.rotationType)
{
}

template<typename Point_t>
inline Orientation::Type OrientationBase<Point_t>::type() const
{
    return rotationType;
}

/*! \} */ // end of rotation

/*! \} */ // end of algebra

/*! \} */ // end of math

} // End namespace tl

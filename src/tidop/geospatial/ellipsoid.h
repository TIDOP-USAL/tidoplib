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

#include <string>

namespace tl
{

/*!
 * \addtogroup geospatial
 *
 * \{
 */

class Ellipsoid
{

public:

  std::string name;
  double a; // semi-major axis;
  double b; // semi-minor axis;

public:

  Ellipsoid(std::string ellipsoidName,
            double semiMajorAxis,
            double semiMinorAxis)
    : name(std::move(ellipsoidName)),
      a(semiMajorAxis),
      b(semiMinorAxis)
  {
  }

  /*!
   * \brief Flattening
   * \f[ f = \frac{a - b}{a} \f]
   */
  double flattening() const
  {
    return (a - b) / a;
  }

  /*!
   * \brief Inverse flattening
   * \f[ 1/f = \frac{a}{a - b} \f]
   * 
   * https://docs.ogc.org/is/18-010r7/18-010r7.html#66 
   * ISO 19111 allows an oblate ellipsoid to be defined through semi-major
   * axis (a) and either semi-minor axis (b) or inverse flattening (1/f). 
   * If semi-minor axis is used as the second defining parameter the value
   * for inverse flattening to be shown in the WKT string should be calculated 
   * from 1/f  =  a / (a – b).
   *
   * ISO 19111 also allows for the earth model to be a sphere, for which
   * 1/f is infinite. In this document if the earth model is a sphere 
   * <inverse flattening> shall be given an artificial value of zero.
   */
  double inverseFlattening() const
  {
    if (a == b) return 0.;
    return a / (a - b);
  }

  /*!
   * \brief Second flattening
   * \f[ f^{'} = \frac{a - b}{b} \f]
   */
  double secondFlattening() const
  {
    return (a - b) / b;
  }

  /*!
   * \brief Third flattening
   * \f[ n = \frac{a - b}{a + b} \f]
   */
  double thirdFlattening() const
  {
    return (a - b) / (a + b);
  }

  /*!
   * \brief Eccentricity
   * \f[ e = \sqrt{\frac{a^2 - b^2}{a^2}} \f]
   */
  double eccentricity() const
  {
    return std::sqrt((a * a - b * b) / (a * a));
  }

  /*!
   * \brief Second eccentricity
   * \f[ e^{'} = \sqrt{\frac{a^2 - b^2}{b^2}} \f]
   */
  double secondEccentricity() const
  {
    return std::sqrt((a * a - b * b) / (b * b));
  }

  /*!
   * \brief Third eccentricity
   * \f[ e^{''} = \sqrt{\frac{a^2 - b^2}{a^2 + b^2}} \f]
   */
  double thirdEccentricity() const
  {
    return std::sqrt(a * a - b * b) / std::sqrt(a * a + b * b);
  }

};

/*! \} */ // end of geospatial

} // End namespace tl
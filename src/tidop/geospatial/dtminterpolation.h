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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
#ifndef TL_GEOSPATIAL_INTERPOLATION_H
#define TL_GEOSPATIAL_INTERPOLATION_H

#include "config_tl.h"

#include <vector>
#include <memory>
#include <mutex>

#include "tidop/core/defs.h"

#include "tidop/core/messages.h"
#include "tidop/geometry/entities/window.h"


namespace tl
{


/* ---------------------------------------------------------------------------------- */

namespace geospatial
{


class TL_EXPORT Interpolation
{

public:

  enum class Parameter
  {
    radius = (0 << 0),
    radius1 = (1 << 0),
    radius2 = (1 << 1),
    angle = (1 << 2),
    min_points = (1 << 3),
    max_points = (1 << 4),
    power = (1 << 5),
    smoothing = (1 << 6)
  };


  enum class Algorithm
  {
    linear  = static_cast<std::underlying_type<Algorithm>::type>(Parameter::radius),
    nearest = static_cast<std::underlying_type<Algorithm>::type>(Parameter::radius1) |
              static_cast<std::underlying_type<Algorithm>::type>(Parameter::radius2) |
              static_cast<std::underlying_type<Algorithm>::type>(Parameter::angle),
    average = static_cast<std::underlying_type<Algorithm>::type>(Parameter::radius1) |
              static_cast<std::underlying_type<Algorithm>::type>(Parameter::radius2) |
              static_cast<std::underlying_type<Algorithm>::type>(Parameter::angle) |
              static_cast<std::underlying_type<Algorithm>::type>(Parameter::min_points),
    invdist = static_cast<std::underlying_type<Algorithm>::type>(Parameter::radius1) |
              static_cast<std::underlying_type<Algorithm>::type>(Parameter::radius2) |
              static_cast<std::underlying_type<Algorithm>::type>(Parameter::angle) |
              static_cast<std::underlying_type<Algorithm>::type>(Parameter::min_points) |
              static_cast<std::underlying_type<Algorithm>::type>(Parameter::max_points) |
              static_cast<std::underlying_type<Algorithm>::type>(Parameter::power) |
              static_cast<std::underlying_type<Algorithm>::type>(Parameter::smoothing),
    invdistnn = static_cast<std::underlying_type<Algorithm>::type>(Parameter::radius) |
                static_cast<std::underlying_type<Algorithm>::type>(Parameter::min_points) |
                static_cast<std::underlying_type<Algorithm>::type>(Parameter::max_points) |
                static_cast<std::underlying_type<Algorithm>::type>(Parameter::power) |
                static_cast<std::underlying_type<Algorithm>::type>(Parameter::smoothing)
  };

  typedef std::map<Interpolation::Parameter, std::string>::iterator parameter_iterator;
  typedef std::map<Interpolation::Parameter, std::string>::const_iterator parameter_const_iterator;

public:

  Interpolation(){}
  virtual ~Interpolation() = default;


  virtual Algorithm algorithm() const = 0;
  virtual std::string algorithmName() const = 0;

  virtual parameter_iterator parametersBegin() = 0;
  virtual parameter_const_iterator parametersBegin() const = 0;
  virtual parameter_iterator parametersEnd() = 0;
  virtual parameter_const_iterator parametersEnd() const = 0;
  virtual std::string parameterName(Parameter parameter) const = 0;
  virtual bool existParameter(Parameter parameter) const = 0;
  virtual std::string parameter(Parameter parameter) const = 0;
  virtual void setParameter(Parameter parameter, std::string value) = 0;
};


class TL_EXPORT InterpolationBase
  : public Interpolation
{

public:

  InterpolationBase(Algorithm algorithm);
  virtual ~InterpolationBase() override;

  Algorithm algorithm() const override;
  parameter_iterator parametersBegin() override;
  parameter_const_iterator parametersBegin() const override;
  parameter_iterator parametersEnd() override;
  parameter_const_iterator parametersEnd() const override;
  std::string parameterName(Parameter parameter) const;
  bool existParameter(Parameter parameter) const override;
  std::string parameter(Parameter parameter) const override;
  void setParameter(Parameter parameter, std::string value) override;

protected:

  void init();
  Algorithm convertFlags(Parameter parameter);

protected:

  tl::EnumFlags<Algorithm> mAlgorithm;
  std::map<Parameter, std::string> mParameters;

};


/* ------------------------------------------------------------------ */



class TL_EXPORT InterpolationFactory
{

private:

  InterpolationFactory() {}

public:

  static std::shared_ptr<Interpolation> create(const std::string &algorithmName);
  static std::shared_ptr<Interpolation> create(Interpolation::Algorithm algorithm);
};



} // End namespace geospatial


} // End namespace tl


#endif // TL_GEOSPATIAL_INTERPOLATION_H

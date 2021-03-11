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
 
#include "dtminterpolation.h"

#include "tidop/core/utils.h"


#include "gdal.h"
#include "gdalgrid.h"


namespace tl
{

namespace geospatial
{

/* ---------------------------------------------------------------------------------- */


InterpolationBase::InterpolationBase(Algorithm algorithm)
  : mAlgorithm(algorithm)
{

}

InterpolationBase::~InterpolationBase()
{

}

Interpolation::Algorithm InterpolationBase::algorithm() const
{
  return mAlgorithm.flags();
}

void InterpolationBase::init()
{

}

Interpolation::Algorithm InterpolationBase::convertFlags(Parameter parameter)
{
  return static_cast<Algorithm>(static_cast<std::underlying_type<Algorithm>::type>(parameter));
}

Interpolation::parameter_iterator InterpolationBase::parametersBegin()
{
  return mParameters.begin();
}

Interpolation::parameter_const_iterator InterpolationBase::parametersBegin() const
{
  return mParameters.cbegin();
}

Interpolation::parameter_iterator InterpolationBase::parametersEnd()
{
  return parameter_iterator();
}

Interpolation::parameter_const_iterator InterpolationBase::parametersEnd() const
{
  return mParameters.end();
}

std::string InterpolationBase::parameterName(Parameter parameter) const
{
  std::string parameter_name;
  switch (parameter)
  {
  case Interpolation::Parameter::radius:
    parameter_name = "radius";
    break;
  case Interpolation::Parameter::radius1:
    parameter_name = "radius1";
    break;
  case Interpolation::Parameter::radius2:
    parameter_name = "radius2";
    break;
  case Interpolation::Parameter::angle:
    parameter_name = "angle";
    break;
  case Interpolation::Parameter::min_points:
    parameter_name = "min_points";
    break;
  case Interpolation::Parameter::max_points:
    parameter_name = "max_points";
    break;
  case Interpolation::Parameter::power:
    parameter_name = "power";
    break;
  case Interpolation::Parameter::smoothing:
    parameter_name = "smoothing";
    break;
  }
  return parameter_name;
}

bool InterpolationBase::existParameter(Parameter parameter) const
{
  return mParameters.find(parameter) != mParameters.end();
}

std::string InterpolationBase::parameter(Parameter parameter) const
{
  return this->existParameter(parameter) ? mParameters.at(parameter) : std::string("0");
}

void InterpolationBase::setParameter(Parameter parameter, std::string value)
{
  if (this->existParameter(parameter))
    mParameters.at(parameter) = value;
}



/* ---------------------------------------------------------------------------------- */


class InterpolationLinear
  : public InterpolationBase
{

public:

  InterpolationLinear()
    : InterpolationBase(Algorithm::linear)
  {
    mParameters[Parameter::radius] = "-1";
  }

  ~InterpolationLinear() override
  {
  }

  std::string algorithmName() const override
  {
    return std::string("linear");
  }

private:
  
  GDALGridAlgorithm *mAlgorithm;

};


class InterpolationNearest
  : public InterpolationBase
{

public:

  InterpolationNearest()
    : InterpolationBase(Algorithm::nearest)
  {
    mParameters[Parameter::radius1] = "0.0";
    mParameters[Parameter::radius2] = "0.0";
    mParameters[Parameter::angle] = "0.0";
  }

  ~InterpolationNearest() override
  { 
  }

  std::string algorithmName() const override
  {
    return std::string("nearest");
  }
};



class InterpolationAverage
  : public InterpolationBase
{

public:

  InterpolationAverage()    
    : InterpolationBase(Algorithm::average)
  {
    mParameters[Parameter::radius1] = "0.0";
    mParameters[Parameter::radius2] = "0.0";
    mParameters[Parameter::angle] = "0.0";
    mParameters[Parameter::min_points] = "0";
  }

  ~InterpolationAverage() override
  {
  }

  std::string algorithmName() const override
  {
    return std::string("average");
  }

};



class InterpolationInverseDistanceToPower
  : public InterpolationBase
{

public:

  InterpolationInverseDistanceToPower()
    : InterpolationBase(Algorithm::invdist)
  {
    mParameters[Parameter::power] = "2.0";
    mParameters[Parameter::smoothing] = "0.0";
    mParameters[Parameter::radius1] = "0.0";
    mParameters[Parameter::radius2] = "0.0";
    mParameters[Parameter::angle] = "0.0";
    mParameters[Parameter::min_points] = "0";
    mParameters[Parameter::max_points] = "0";

  }

  ~InterpolationInverseDistanceToPower() override
  {
  }

  std::string algorithmName() const override
  {
    return std::string("invdist");
  }
};



class InterpolationInverseDistanceToPowerNearestNeighbor
  : public InterpolationBase
{

public:

  InterpolationInverseDistanceToPowerNearestNeighbor()
    : InterpolationBase(Algorithm::invdistnn)
  {
    mParameters[Parameter::power] = "2.0";
    mParameters[Parameter::smoothing] = "0.0";
    mParameters[Parameter::radius] = "1.0";
    mParameters[Parameter::angle] = "0.0";
    mParameters[Parameter::min_points] = "0";
    mParameters[Parameter::max_points] = "12";
  }

  ~InterpolationInverseDistanceToPowerNearestNeighbor() override
  {
  }

  std::string algorithmName() const override
  {
    return std::string("invdistnn");
  }

};




/* ---------------------------------------------------------------------------------- */




std::shared_ptr<Interpolation> InterpolationFactory::create(const std::string &algorithmName)
{
  std::shared_ptr<Interpolation> interpolation;
  if (algorithmName.compare("linear") == 0) {
    interpolation = std::make_shared<InterpolationLinear>();
  } else if (algorithmName.compare("nearest") == 0) {
		interpolation = std::make_shared<InterpolationNearest>();
  } else if (algorithmName.compare("average") == 0) {
		interpolation = std::make_shared<InterpolationAverage>();
  } else if (algorithmName.compare("invdist") == 0) {
    interpolation = std::make_shared<InterpolationInverseDistanceToPower>();
  } else if (algorithmName.compare("invdistnn") == 0) {
		interpolation = std::make_shared<InterpolationInverseDistanceToPowerNearestNeighbor>();
  }
  return interpolation;
}

std::shared_ptr<Interpolation> InterpolationFactory::create(Interpolation::Algorithm algorithm)
{
  std::shared_ptr<Interpolation> interpolation;
	switch (algorithm) {
		case tl::geospatial::Interpolation::Algorithm::linear:
			interpolation = std::make_shared<InterpolationLinear>();
			break;
		case tl::geospatial::Interpolation::Algorithm::nearest:
			interpolation = std::make_shared<InterpolationNearest>();
			break;
		case tl::geospatial::Interpolation::Algorithm::average:
			interpolation = std::make_shared<InterpolationAverage>();
			break;
		case tl::geospatial::Interpolation::Algorithm::invdist:
			interpolation = std::make_shared<InterpolationInverseDistanceToPower>();
			break;
		case tl::geospatial::Interpolation::Algorithm::invdistnn:
			interpolation = std::make_shared<InterpolationInverseDistanceToPowerNearestNeighbor>();
			break;
		default:
			break;
	}
  return interpolation;
}

} // End namespace  geospatial

} // End namespace tl


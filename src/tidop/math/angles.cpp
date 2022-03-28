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

#include "angles.h"

namespace tl
{

namespace math
{
  

double degreesToDecimalDegrees(int degrees, int minutes, double seconds)
{
  return isNegative(degrees) * (abs(degrees) +
                                minutes / static_cast<double>(consts::degrees_to_minutes) +
                                seconds / static_cast<double>(consts::degrees_to_seconds));
}

double degreesToRadians(int degrees, int minutes, double seconds)
{
  return decimalDegreesToRadians(degreesToDecimalDegrees(degrees, minutes, seconds));
}

double degreesToGradians(int degrees, int minutes, double seconds)
{
  return decimalDegreesToGradians(degreesToDecimalDegrees(degrees, minutes, seconds));
}

void decimalDegreesToDegrees(double decimalDegrees, int &degrees, int &minutes, double &seconds)
{
  seconds = decimalDegrees * consts::degrees_to_seconds;
  int s = static_cast<int>(seconds);
  seconds -= s;
  degrees = s / consts::degrees_to_seconds;
  s = std::abs(s % consts::degrees_to_seconds);
  minutes = s / consts::minutes_to_seconds;
  s %= consts::minutes_to_seconds;
  seconds += s;
}

double decimalDegreesToRadians(double decimalDegrees)
{
  return decimalDegrees * consts::deg_to_rad<double>;
}

double decimalDegreesToGradians(double decimalDegrees)
{
  return decimalDegrees * consts::deg_to_grad<double>;
}

void radiansToDegrees(double rad, int &degrees, int &minutes, double &seconds)
{
  decimalDegreesToDegrees(radiansToDecimalDegrees(rad), degrees, minutes, seconds);
}

double radiansToDecimalDegrees(double radians)
{ 
  return radians * consts::rad_to_deg<double>;
}

double radiansToGradians(double radians)
{
  return radians * consts::rad_to_grad<double>;
}

void gradiansToDegrees(double gradians, int &degrees, int &minutes, double &seconds)
{
  decimalDegreesToDegrees(gradiansToDecimalDegrees(gradians), degrees, minutes, seconds);
}

double gradiansToDecimalDegrees(double gradians)
{
  return gradians * consts::grad_to_deg<double>;
}

double gradiansToRadians(double gradians)
{
  return gradians * consts::grad_to_rad<double>;
}


} // Fin namespace math

} // Fin namespace tl

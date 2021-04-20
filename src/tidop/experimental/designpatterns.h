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
 
#ifndef TL_DESING_PATTERNS_H
#define TL_DESING_PATTERNS_H

#include <mutex>

namespace tl
{

namespace experimental
{

/*! \defgroup DesingPatterns Patrones de diseño
 *
 *  \{
 */

//class Singleton
//{

//protected:

//  static Singleton *sObj;
//  static std::mutex sMutex;

//private:

//  Singleton();

//public:

//  virtual ~Singleton();
//  Singleton(Singleton const&) = delete;
//  void operator=(Singleton const&) = delete;
//  static Singleton *getInstance();
  
//private:

//  virtual Singleton *create() = 0;
//};

/*! \} */ // DesingPatterns

} // End namespace experimental

} // End namespace tl

#endif // TL_DESING_PATTERNS_H

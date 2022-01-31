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

#ifndef TL_CORE_UTILS_XML_H
#define TL_CORE_UTILS_XML_H

#include "config_tl.h"

#include "tidop/core/defs.h"

#include <memory>
#include <mutex>


namespace tl
{

namespace internal
{
struct Node;
}

/*! \addtogroup core
 *  \{
 */


class XmlNode
{

public:

	XmlNode();
  XmlNode(const XmlNode &node);
	~XmlNode();

  XmlNode next();
  XmlNode child();

private:

  std::shared_ptr<internal::Node> *mNode;
};




/*!
 * \brief XMLReader
 *
 */
class TL_EXPORT XMLReader
{

public:
  
  XMLReader();
  ~XMLReader() = default;

};


/*!
 * \brief XMLReader
 *
 */
class TL_EXPORT XMLWriter
{

public:
  
  XMLWriter();
  ~XMLWriter() = default;

};


/*! \} */ // end of core

} // End namespace tl


#endif // TL_CORE_UTILS_GDAL_H

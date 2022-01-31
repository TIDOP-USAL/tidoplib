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

#include "tidop/core/xml.h"

#include "tidop/core/messages.h"
#include "tidop/core/console.h"
#include "tidop/core/exception.h"

#ifdef TL_HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "gdal.h"
TL_DEFAULT_WARNINGS
#endif // TL_HAVE_GDAL


namespace tl
{

//#ifdef TL_HAVE_GDAL

namespace internal
{

struct Node
{
	CPLXMLNode *mCplXmlNode;
	friend class XmlNode;
};

}


XmlNode::XmlNode()
{
}

XmlNode::XmlNode(const XmlNode &node)
	: mNode(node.mNode)
{
}

XmlNode::~XmlNode()
{
}

XmlNode XmlNode::next()
{
	XmlNode node;
	return node;
}

XmlNode XmlNode::child()
{
	return XmlNode();
}




XMLReader::XMLReader()
{
}





XMLWriter::XMLWriter()
{
}


} // End namespace tl


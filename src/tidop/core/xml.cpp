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

class XmlNode
{

public:

	CPLXMLNode *mCplXmlNode;
	friend class XmlNode;
};

}




XmlNodeValue::XmlNodeValue(std::string value)
	: mValue(std::move(value))
{
}

XmlNodeValue::~XmlNodeValue()
{
}

std::string XmlNodeValue::toString()
{
	return std::string();
}

int XmlNodeValue::toInteger()
{
	return 0;
}

float XmlNodeValue::toFloat()
{
	return 0.0f;
}

double XmlNodeValue::toDouble()
{
	return 0.0;
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

XmlNode &XmlNode::next()
{
	//internal::XmlNode node{};
	//node.mCplXmlNode = this->mNode->mCplXmlNode->psNext;
	XmlNode node{};
	node.mNode->mCplXmlNode = this->mNode->mCplXmlNode->psNext;

	return node;
}

XmlNode &XmlNode::child()
{
	XmlNode node{};
	node.mNode->mCplXmlNode = this->mNode->mCplXmlNode->psChild;
	return node;
}

XmlNodeValue XmlNode::value()
{
	return XmlNodeValue(this->mNode->mCplXmlNode->pszValue);
}




XMLReader::XMLReader()
{
	
}

void XMLReader::parse(const tl::Path &file)
{
	CPLXMLNode *xml_node = CPLParseXMLFile(file.toString().c_str());
}



XMLWriter::XMLWriter(const std::string &file)
{
}




} // End namespace tl


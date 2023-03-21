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

#ifdef TL_HAVE_GDAL

#include "tidop/core/messages.h"
#include "tidop/core/console.h"
#include "tidop/core/exception.h"

TL_DISABLE_WARNINGS
#include "gdal.h"
TL_DEFAULT_WARNINGS



namespace tl
{
	
/// \cond

namespace internal
{

class XmlNode
{

public:

	CPLXMLNode *node;
	friend class tl::XmlNode;
};

} // namespace internal

/// \endcond


XmlNodeValue::XmlNodeValue(std::string value)
	: mValue(std::move(value))
{
}

XmlNodeValue::~XmlNodeValue()
{
}

std::string XmlNodeValue::toString()
{
	return mValue;
}

int XmlNodeValue::toInteger()
{
	return std::stoi(mValue);
}

float XmlNodeValue::toFloat()
{
	return std::stof(mValue);
}

double XmlNodeValue::toDouble()
{
	return std::stod(mValue);
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
	//XmlNode node{};
	this->mNode->node = this->mNode->node->psNext;

	return *this;
}

XmlNode &XmlNode::child()
{
	//XmlNode node{};
	//node.mNode->node = this->mNode->node->psChild;
	this->mNode->node = this->mNode->node->psChild;
	return *this;
}

XmlNodeValue XmlNode::value() const
{
	return XmlNodeValue(this->mNode->node->pszValue);
}

XmlNode::Type XmlNode::type() const
{
	Type type; 

	switch(mNode->node->eType) {
		case CPLXMLNodeType::CXT_Element:
			type = XmlNode::Type::element;
			break;
		case CPLXMLNodeType::CXT_Text:
			type = XmlNode::Type::text;
			break;
		case CPLXMLNodeType::CXT_Attribute:
			type = XmlNode::Type::attribute;
			break;
		case CPLXMLNodeType::CXT_Comment:
			type = XmlNode::Type::comment;
			break;
		case CPLXMLNodeType::CXT_Literal:
			type = XmlNode::Type::literal;
			break;
	}

  return type;
}

bool XmlNode::isAttibute() const
{
	return mNode->node->eType == CXT_Attribute;
}

bool XmlNode::isText() const
{
	return mNode->node->eType == CXT_Text;
}

bool XmlNode::isComment() const
{
	return mNode->node->eType == CXT_Comment;
}




XMLReader::XMLReader()
{
	
}

XmlNode XMLReader::parse(const tl::Path &file)
{
	//CPLXMLNode *_xml_node = CPLParseXMLFile(file.toString().c_str());
	XmlNode xml_node;
	xml_node.mNode->node = CPLParseXMLFile(file.toString().c_str());
	return xml_node;
}

XmlNode XMLReader::parse(const std::string &xml)
{
	CPLXMLNode *cpl_xml_node = CPLParseXMLString(xml.c_str());
	XmlNode xml_node;
	xml_node.mNode = std::make_shared<internal::XmlNode>();
	xml_node.mNode->node = cpl_xml_node;
	return xml_node;
}



XMLWriter::XMLWriter(const std::string &file)
{
	unusedParameter(file);
}



} // End namespace tl

#endif //TL_HAVE_GDAL

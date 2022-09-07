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
 

#define BOOST_TEST_MODULE Tidop xml test
#include <boost/test/unit_test.hpp>
#include <tidop/core/xml.h>

#ifdef TL_HAVE_GDAL

using namespace tl;

BOOST_AUTO_TEST_SUITE(XmlTestSuite)


struct XmlTest
{
  XmlTest()
  {
  }

  ~XmlTest()
  {
  }

  void setup()
  {
    text = "<?xml version=\"1.0\" encoding=\"UTF - 8\"?>"
           "<Nodo1>"
           "  <Nodo2>"
           "  </Nodo2>"
           "</Nodo1>";
  }

  void teardown()
  {

  }

  std::string text;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, XmlTest)
{
  XmlNode xml_node;
  {
    XMLReader reader;
    xml_node = reader.parse(text);
  }
  auto &node = xml_node.next();
  std::string node_value = node.value().toString();
  auto &child = xml_node.child();
  std::string child_value = child.value().toString();

}

BOOST_FIXTURE_TEST_CASE(next, XmlTest)
{

}

BOOST_AUTO_TEST_SUITE_END()

#endif
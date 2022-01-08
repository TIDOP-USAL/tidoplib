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
 
#define BOOST_TEST_MODULE Tidop graphic styles test
#include <boost/test/unit_test.hpp>
#include <tidop/graphic/styles.h>


using namespace tl;
using namespace graph;

struct StylePenTest
{
  StylePenTest()
  {
    style = new graph::StylePen;
  }
    
  ~StylePenTest()
  {
    if (style) {
      delete style;
      style = nullptr;
    }
  }

  void setup()
  {
  }

  void teardown()
  {
  }

  graph::StylePen def_style;
  graph::StylePen *style;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, StylePenTest)
{
  graph::Color color;
  BOOST_CHECK(color == def_style.color());
  BOOST_CHECK_EQUAL(1, def_style.width());
  BOOST_CHECK_EQUAL("", def_style.pattern());
  BOOST_CHECK(StylePen::Name::solid == def_style.name());
  BOOST_CHECK(StylePen::Cap::butt == def_style.cap());
  BOOST_CHECK(StylePen::Join::bevel == def_style.join());
  BOOST_CHECK_EQUAL(0, def_style.perpendicularOffset());
  BOOST_CHECK_EQUAL(0, def_style.priorityLevel());
}

BOOST_FIXTURE_TEST_CASE(setColor, StylePenTest)
{
  graph::Color color(graph::Color::Name::beige);
  style->setColor(color);
  BOOST_CHECK(color == style->color());
}

BOOST_FIXTURE_TEST_CASE(setWidth, StylePenTest)
{
  style->setWidth(5);
  BOOST_CHECK_EQUAL(5, style->width());
}

BOOST_FIXTURE_TEST_CASE(setPattern, StylePenTest)
{
  style->setPattern("4px 5px");
  BOOST_CHECK_EQUAL("4px 5px", style->pattern());
}

BOOST_FIXTURE_TEST_CASE(setName, StylePenTest)
{
  style->setName(StylePen::Name::dash_dot_line);
  BOOST_CHECK(StylePen::Name::dash_dot_line == style->name());
}

BOOST_FIXTURE_TEST_CASE(setCap, StylePenTest)
{
  style->setCap(StylePen::Cap::projective);
  BOOST_CHECK(StylePen::Cap::projective == style->cap());
}

BOOST_FIXTURE_TEST_CASE(setJoin, StylePenTest)
{
  style->setJoin(StylePen::Join::rounded);
  BOOST_CHECK(StylePen::Join::rounded == style->join());
}

BOOST_FIXTURE_TEST_CASE(setPerpendicularOffset, StylePenTest)
{
  style->setPerpendicularOffset(25);
  BOOST_CHECK_EQUAL(25, style->perpendicularOffset());
}

BOOST_FIXTURE_TEST_CASE(setPriorityLevel, StylePenTest)
{
  style->setPriorityLevel(15);
  BOOST_CHECK_EQUAL(15, style->priorityLevel());
}



BOOST_AUTO_TEST_SUITE(StyleBrushTestSuite)

struct StyleBrushTest
{
  StyleBrushTest()
  {}

  ~StyleBrushTest()
  {
  }
  
  void setup()
  {
  }

  void teardown()
  {
  }

  StyleBrush def_style;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, StyleBrushTest)
{
  graph::Color color;
  BOOST_CHECK(color == def_style.foreColor());
  BOOST_CHECK(color == def_style.backColor());
  BOOST_CHECK(StyleBrush::Name::solid == def_style.brushName());
  BOOST_CHECK_EQUAL(0., def_style.angle());
  BOOST_CHECK_EQUAL(1., def_style.scalingFactor());
  BOOST_CHECK_EQUAL(0., def_style.spacingX());
  BOOST_CHECK_EQUAL(0., def_style.spacingY());
  BOOST_CHECK_EQUAL(0, def_style.priorityLevel());
}

BOOST_FIXTURE_TEST_CASE(setForeColor, StyleBrushTest)
{
  graph::Color color(graph::Color::Name::indigo);
  def_style.setForeColor(color);
  BOOST_CHECK(color == def_style.foreColor());
}

BOOST_FIXTURE_TEST_CASE(setBackColor, StyleBrushTest)
{
  graph::Color color(graph::Color::Name::indigo);
  def_style.setBackColor(color);
  BOOST_CHECK(color == def_style.backColor());
}

BOOST_FIXTURE_TEST_CASE(setBrushName, StyleBrushTest)
{
  def_style.setBrushName(StyleBrush::Name::null);
  BOOST_CHECK(StyleBrush::Name::null == def_style.brushName());
}

BOOST_FIXTURE_TEST_CASE(setAngle, StyleBrushTest)
{
  def_style.setAngle(.45);
  BOOST_CHECK_EQUAL(0.45, def_style.angle());
}

BOOST_FIXTURE_TEST_CASE(setScalingFactor, StyleBrushTest)
{
  def_style.setScalingFactor(2.);
  BOOST_CHECK_EQUAL(2., def_style.scalingFactor());
}

BOOST_FIXTURE_TEST_CASE(setSpacing, StyleBrushTest)
{
  def_style.setSpacing(2., 5.);
  BOOST_CHECK_EQUAL(2., def_style.spacingX());
  BOOST_CHECK_EQUAL(5., def_style.spacingY());
}

BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_SUITE(StyleSymbolTestSuite)


struct StyleSymbolTest
{
  StyleSymbolTest()
    : style(new StyleSymbol)
  {}

  ~StyleSymbolTest()
  {
    if (style) {
      delete style;
      style = nullptr;
    }
  }

  void setup()
  {
    
  }

  void teardown()
  {

  }

  StyleSymbol def_style;
  StyleSymbol *style;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, StyleSymbolTest)
{
  BOOST_CHECK_EQUAL(0., def_style.angle());
  BOOST_CHECK(StyleSymbol::Name::cross == def_style.name());
  graph::Color color;
  BOOST_CHECK(color == def_style.color());
  BOOST_CHECK(color == def_style.outlineColor());
  BOOST_CHECK_EQUAL(1., def_style.scalingFactor());
  BOOST_CHECK_EQUAL(0., def_style.offsetX());
  BOOST_CHECK_EQUAL(0., def_style.offsetY());
  BOOST_CHECK_EQUAL(0, def_style.priorityLevel());
}

BOOST_FIXTURE_TEST_CASE(setAngle, StyleSymbolTest)
{
  style->setAngle(0.26);
  BOOST_CHECK_EQUAL(0.26, style->angle());
}

BOOST_FIXTURE_TEST_CASE(setName, StyleSymbolTest)
{
  style->setName(StyleSymbol::Name::circle);
  BOOST_CHECK(StyleSymbol::Name::circle == style->name());
}

BOOST_FIXTURE_TEST_CASE(setColor, StyleSymbolTest)
{
  graph::Color color(graph::Color::Name::gainsboro);
  style->setColor(color);
  BOOST_CHECK(color == style->color());
}

BOOST_FIXTURE_TEST_CASE(setOutlineColor, StyleSymbolTest)
{
  graph::Color color(graph::Color::Name::dark_slate_gray);
  style->setOutlineColor(color);
  BOOST_CHECK(color == style->outlineColor());
}

BOOST_FIXTURE_TEST_CASE(setScalingFactor, StyleSymbolTest)
{
  style->setScalingFactor(2.);
  BOOST_CHECK_EQUAL(2., style->scalingFactor());
}

BOOST_FIXTURE_TEST_CASE(setOffset, StyleSymbolTest)
{
  style->setOffset(5., 2.);
  BOOST_CHECK_EQUAL(5., style->offsetX());
  BOOST_CHECK_EQUAL(2., style->offsetY());
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(StyleLabelTestSuite)

struct StyleLabelTest
{
  StyleLabelTest()
    : style(new StyleLabel)
  {}

  ~StyleLabelTest()
  {
    if (style) {
      delete style;
      style = nullptr;
    }
  }

  void setup()
  {

  }

  void teardown()
  {
  }

  StyleLabel def_style;
  StyleLabel *style;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, StyleLabelTest)
{
  BOOST_CHECK_EQUAL("", def_style.text());
  BOOST_CHECK_EQUAL(0., def_style.angle());
  graph::Color color;
  BOOST_CHECK(color == def_style.foregroundColor());
  BOOST_CHECK(color == def_style.backgroundColor());
  BOOST_CHECK(color == def_style.outlineColor());
  BOOST_CHECK(color == def_style.shadowColor());
  BOOST_CHECK_EQUAL(100., def_style.stretch());
  BOOST_CHECK(StyleLabel::LabelPlacement::p == def_style.labelPlacement());
  BOOST_CHECK((StyleLabel::AnchorPosition::vertical_baseline | StyleLabel::AnchorPosition::horizontal_left) ==
            def_style.anchorPosition());
  BOOST_CHECK_EQUAL(0., def_style.offsetX());
  BOOST_CHECK_EQUAL(0., def_style.offsetY());
  BOOST_CHECK_EQUAL(0., def_style.perpendicularOffset());
}

BOOST_FIXTURE_TEST_CASE(setText, StyleLabelTest)
{
  style->setText("label");
  BOOST_CHECK_EQUAL("label", style->text());
}

BOOST_FIXTURE_TEST_CASE(setAngle, StyleLabelTest)
{
  style->setAngle(25.5);
  BOOST_CHECK_EQUAL(25.5, style->angle());
}

BOOST_FIXTURE_TEST_CASE(setForegroundColor, StyleLabelTest)
{
  graph::Color color(graph::Color::Name::fire_brick);
  style->setForegroundColor(color);
  BOOST_CHECK(color == style->foregroundColor());
}

BOOST_FIXTURE_TEST_CASE(setBackgroundColor, StyleLabelTest)
{
  graph::Color color(graph::Color::Name::indigo);
  style->setBackgroundColor(color);
  BOOST_CHECK(color == style->backgroundColor());
}

BOOST_FIXTURE_TEST_CASE(setOutlineColor, StyleLabelTest)
{
  graph::Color color(graph::Color::Name::saddle_brown);
  style->setOutlineColor(color);
  BOOST_CHECK(color == style->outlineColor());
}

BOOST_FIXTURE_TEST_CASE(setShadowColor, StyleLabelTest)
{
  graph::Color color(graph::Color::Name::azure);
  style->setShadowColor(color);
  BOOST_CHECK(color == style->shadowColor());
}

BOOST_FIXTURE_TEST_CASE(setStretch, StyleLabelTest)
{
  style->setStretch(25.);
  BOOST_CHECK_EQUAL(25., style->stretch());
}

BOOST_FIXTURE_TEST_CASE(setLabelPlacement, StyleLabelTest)
{
  style->setLabelPlacement(StyleLabel::LabelPlacement::l);
  BOOST_CHECK(StyleLabel::LabelPlacement::l == style->labelPlacement());
}

BOOST_FIXTURE_TEST_CASE(setAnchorPosition, StyleLabelTest)
{
  style->setAnchorPosition(StyleLabel::AnchorPosition::vertical_center | StyleLabel::AnchorPosition::horizontal_left);
  BOOST_CHECK((StyleLabel::AnchorPosition::vertical_center | StyleLabel::AnchorPosition::horizontal_left) == style->anchorPosition());
}

BOOST_FIXTURE_TEST_CASE(setOffset, StyleLabelTest)
{
  style->setOffset(5., 2.);
  BOOST_CHECK_EQUAL(5., style->offsetX());
  BOOST_CHECK_EQUAL(2., style->offsetY());
}

BOOST_FIXTURE_TEST_CASE(setPerpendicularOffset, StyleLabelTest)
{
  style->setPerpendicularOffset(10.);
  BOOST_CHECK_EQUAL(10., style->perpendicularOffset());
}

BOOST_AUTO_TEST_SUITE_END()
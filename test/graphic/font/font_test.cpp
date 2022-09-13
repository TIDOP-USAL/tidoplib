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

#define BOOST_TEST_MODULE Tidop graphic font test
#include <boost/test/unit_test.hpp>
#include <tidop/graphic/font.h>

using namespace tl;
using namespace graph;

BOOST_AUTO_TEST_SUITE(FontTestSuite)

struct FontTest
{

  FontTest()
  {
    font = new Font;
  }

  ~FontTest()
  {
    if (font) {
      delete font;
      font = nullptr;
    }
  }

  Font defFont;
  Font *font;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, FontTest)
{
  BOOST_CHECK_EQUAL("Arial", defFont.name());
  BOOST_CHECK_EQUAL(10, defFont.size());
  BOOST_CHECK_EQUAL(false, defFont.isBold());
  BOOST_CHECK_EQUAL(false, defFont.isItalic());
  BOOST_CHECK_EQUAL(false, defFont.isUnderline());
  BOOST_CHECK_EQUAL(false, defFont.isStrikethrough());
}

BOOST_FIXTURE_TEST_CASE(setName, FontTest)
{
  font->setName("Times New Roman");
  BOOST_CHECK_EQUAL("Times New Roman", font->name());
}

BOOST_FIXTURE_TEST_CASE(setSize, FontTest)
{
  font->setSize(14);
  BOOST_CHECK_EQUAL(14, font->size());
}

BOOST_FIXTURE_TEST_CASE(setBold, FontTest)
{
  font->setBold(true);
  BOOST_CHECK_EQUAL(true, font->isBold());
}

BOOST_FIXTURE_TEST_CASE(setItalic, FontTest)
{
  font->setItalic(true);
  BOOST_CHECK_EQUAL(true, font->isItalic());
}

BOOST_FIXTURE_TEST_CASE(setUnderline, FontTest)
{
  font->setUnderline(true);
  BOOST_CHECK_EQUAL(true, font->isUnderline());
}

BOOST_FIXTURE_TEST_CASE(setStrikethrough, FontTest)
{
  font->setStrikethrough(true);
  BOOST_CHECK_EQUAL(true, font->isStrikethrough());
}

BOOST_FIXTURE_TEST_CASE(setStyle, FontTest)
{
  font->setStyle(Font::Style::bold);
  BOOST_CHECK_EQUAL(true, font->isBold());
  BOOST_CHECK_EQUAL(false, font->isItalic());

  font->setStyle(Font::Style::italic);
  BOOST_CHECK_EQUAL(false, font->isBold());
  BOOST_CHECK_EQUAL(true, font->isItalic());

  font->setStyle(Font::Style::normal);
  BOOST_CHECK_EQUAL(false, font->isBold());
  BOOST_CHECK_EQUAL(false, font->isItalic());

  font->setStyle(Font::Style::bold | Font::Style::italic);
  BOOST_CHECK_EQUAL(true, font->isBold());
  BOOST_CHECK_EQUAL(true, font->isItalic());
}

BOOST_AUTO_TEST_SUITE_END()
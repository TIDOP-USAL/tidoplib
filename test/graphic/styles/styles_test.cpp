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

struct StylePenTest
{
    StylePenTest()
    {
        style = new Pen;
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

    Pen def_style;
    Pen *style;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, StylePenTest)
{
    Color color;
    BOOST_CHECK(color == def_style.color());
    BOOST_CHECK_EQUAL(1, def_style.width());
    BOOST_CHECK_EQUAL("", def_style.pattern());
    BOOST_CHECK(Pen::PredefinedPattern::solid == def_style.predefinedPattern());
    BOOST_CHECK(Pen::Cap::butt == def_style.cap());
    BOOST_CHECK(Pen::Join::bevel == def_style.join());
    BOOST_CHECK_EQUAL(0, def_style.perpendicularOffset());
}

BOOST_FIXTURE_TEST_CASE(setColor, StylePenTest)
{
    Color color(Color::Name::beige);
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

BOOST_FIXTURE_TEST_CASE(setPredefinedPattern, StylePenTest)
{
    style->setPredefinedPattern(Pen::PredefinedPattern::dash_dot_line);
    BOOST_CHECK(Pen::PredefinedPattern::dash_dot_line == style->predefinedPattern());
}

BOOST_FIXTURE_TEST_CASE(setCap, StylePenTest)
{
    style->setCap(Pen::Cap::projective);
    BOOST_CHECK(Pen::Cap::projective == style->cap());
}

BOOST_FIXTURE_TEST_CASE(setJoin, StylePenTest)
{
    style->setJoin(Pen::Join::rounded);
    BOOST_CHECK(Pen::Join::rounded == style->join());
}

BOOST_FIXTURE_TEST_CASE(setPerpendicularOffset, StylePenTest)
{
    style->setPerpendicularOffset(25);
    BOOST_CHECK_EQUAL(25, style->perpendicularOffset());
}



BOOST_AUTO_TEST_SUITE(StyleBrushTestSuite)

struct StyleBrushTest
{
    StyleBrushTest()
    {
    }

    ~StyleBrushTest()
    {
    }

    void setup()
    {
        brush_solid = Brush(Color(Color::Name::azure), Color(Color::Name::deep_pink));
        brush_hatch = Brush(Color(Color::Name::red), Color(Color::Name::blue), Brush::Style::hatch);
        brush_no_fill = Brush(Color(Color::Name::green), Color(Color::Name::dark_green), Brush::Style::no_fill);
        BrushPattern pattern(BrushPattern::HatchType::cross, 0.0, 2.0);
        brush_hatch_cross = Brush(Color(Color::Name::green), Color(Color::Name::dark_green), pattern);
    }

    void teardown()
    {
    }

    Brush def_style;
    Brush brush_solid;
    Brush brush_hatch;
    Brush brush_no_fill;
    Brush brush_hatch_cross;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, StyleBrushTest)
{
    Color color;
    BOOST_CHECK(color == def_style.foregroundColor());
    BOOST_CHECK(color == def_style.backgroundColor());
    BOOST_CHECK(Brush::Style::solid == def_style.style());
    BOOST_CHECK(!def_style.pattern().has_value());
}

BOOST_FIXTURE_TEST_CASE(brush_solid_constructor, StyleBrushTest)
{
    BOOST_CHECK(Color(Color::Name::azure) == brush_solid.foregroundColor());
    BOOST_CHECK(Color(Color::Name::deep_pink) == brush_solid.backgroundColor());
    BOOST_CHECK(Brush::Style::solid == brush_solid.style());
    BOOST_CHECK(!brush_solid.pattern().has_value());
}

BOOST_FIXTURE_TEST_CASE(brush_hatch_constructor, StyleBrushTest)
{
    BOOST_CHECK(Color(Color::Name::red) == brush_hatch.foregroundColor());
    BOOST_CHECK(Color(Color::Name::blue) == brush_hatch.backgroundColor());
    BOOST_CHECK(Brush::Style::hatch == brush_hatch.style());
    auto &brush_pattern = brush_hatch.pattern();
    BOOST_CHECK(brush_pattern.has_value());
    BOOST_CHECK(BrushPattern::HatchType::horizontal == brush_pattern->hatchType());
    BOOST_CHECK_EQUAL(0., brush_pattern->angle());
    BOOST_CHECK_EQUAL(1., brush_pattern->scalingFactor());
    BOOST_CHECK_EQUAL(0., brush_pattern->spacing().x());
    BOOST_CHECK_EQUAL(0., brush_pattern->spacing().y());
}

BOOST_FIXTURE_TEST_CASE(brush_null_constructor, StyleBrushTest)
{
    BOOST_CHECK(Color(Color::Name::green) == brush_no_fill.foregroundColor());
    BOOST_CHECK(Color(Color::Name::dark_green) == brush_no_fill.backgroundColor());
    BOOST_CHECK(Brush::Style::no_fill == brush_no_fill.style());
    BOOST_CHECK(!brush_no_fill.pattern().has_value());
}

BOOST_FIXTURE_TEST_CASE(brush_hatch_cross_constructor, StyleBrushTest)
{
    BOOST_CHECK(Color(Color::Name::green) == brush_hatch_cross.foregroundColor());
    BOOST_CHECK(Color(Color::Name::dark_green) == brush_hatch_cross.backgroundColor());
    BOOST_CHECK(Brush::Style::hatch == brush_hatch_cross.style());
    auto &brush_pattern = brush_hatch_cross.pattern();
    BOOST_CHECK(brush_pattern.has_value());
    BOOST_CHECK(BrushPattern::HatchType::cross == brush_pattern->hatchType());
    BOOST_CHECK_EQUAL(0., brush_pattern->angle());
    BOOST_CHECK_EQUAL(2., brush_pattern->scalingFactor());
    BOOST_CHECK_EQUAL(0., brush_pattern->spacing().x());
    BOOST_CHECK_EQUAL(0., brush_pattern->spacing().y());
}

BOOST_FIXTURE_TEST_CASE(setForegroundColor, StyleBrushTest)
{
    Color color(Color::Name::indigo);
    def_style.setForegroundColor(color);
    BOOST_CHECK(color == def_style.foregroundColor());
}

BOOST_FIXTURE_TEST_CASE(setBackgroundColor, StyleBrushTest)
{
    Color color(Color::Name::indigo);
    def_style.setBackgroundColor(color);
    BOOST_CHECK(color == def_style.backgroundColor());
}

BOOST_FIXTURE_TEST_CASE(setPattern, StyleBrushTest)
{
    BrushPattern pattern;
    pattern.setAngle(45.0);
    pattern.setScalingFactor(2.0);
    pattern.setHatchType(BrushPattern::HatchType::cross);
    pattern.setSpacing(2.0, 5.0);

    def_style.setPattern(pattern);

    BOOST_CHECK(Brush::Style::hatch == def_style.style());
    auto &brush_pattern = def_style.pattern();

    BOOST_CHECK(pattern == brush_pattern);
}

BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_SUITE(StyleSymbolTestSuite)


struct StyleSymbolTest
{
    StyleSymbolTest()
        : style(new Symbol)
    {
    }

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

    Symbol def_style;
    Symbol *style;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, StyleSymbolTest)
{
    BOOST_CHECK_EQUAL(0., def_style.angle());
    BOOST_CHECK(Symbol::Shape::cross == def_style.shape());
    Color color;
    BOOST_CHECK(color == def_style.color());
    BOOST_CHECK(color == def_style.outlineColor());
    BOOST_CHECK_EQUAL(1., def_style.scalingFactor());
    BOOST_CHECK_EQUAL(0., def_style.offset().x());
    BOOST_CHECK_EQUAL(0., def_style.offset().y());
}

BOOST_FIXTURE_TEST_CASE(setAngle, StyleSymbolTest)
{
    style->setAngle(0.26);
    BOOST_CHECK_EQUAL(0.26, style->angle());
}

BOOST_FIXTURE_TEST_CASE(setShape, StyleSymbolTest)
{
    style->setShape(Symbol::Shape::square);
    BOOST_CHECK(Symbol::Shape::square == style->shape());
}

BOOST_FIXTURE_TEST_CASE(setColor, StyleSymbolTest)
{
    Color color(Color::Name::gainsboro);
    style->setColor(color);
    BOOST_CHECK(color == style->color());
}

BOOST_FIXTURE_TEST_CASE(setOutlineColor, StyleSymbolTest)
{
    Color color(Color::Name::dark_slate_gray);
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
    BOOST_CHECK_EQUAL(5., style->offset().x());
    BOOST_CHECK_EQUAL(2., style->offset().y());
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(StyleLabelTestSuite)

struct StyleLabelTest
{
    StyleLabelTest()
        : style(new Label)
    {
    }

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

    Label def_style;
    Label *style;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, StyleLabelTest)
{
    //BOOST_CHECK_EQUAL("", def_style.text());
    BOOST_CHECK_EQUAL(0., def_style.angle());
    Color color;
    BOOST_CHECK(color == def_style.foregroundColor());
    BOOST_CHECK(color == def_style.backgroundColor());
    BOOST_CHECK(color == def_style.outlineColor());
    BOOST_CHECK(color == def_style.shadowColor());
    BOOST_CHECK_EQUAL(100., def_style.stretchFactor());
    BOOST_CHECK(Label::Placement::point == def_style.placement());
    auto anchor_pos = def_style.anchorPosition();
    BOOST_CHECK(anchor_pos.horizontal() == LabelAnchor::Horizontal::left);
    BOOST_CHECK(anchor_pos.vertical() == LabelAnchor::Vertical::baseline);
    BOOST_CHECK_EQUAL(0., def_style.offset().x());
    BOOST_CHECK_EQUAL(0., def_style.offset().y());
    BOOST_CHECK_EQUAL(0., def_style.perpendicularOffset());
}

//BOOST_FIXTURE_TEST_CASE(setText, StyleLabelTest)
//{
//    style->setText("label");
//    BOOST_CHECK_EQUAL("label", style->text());
//}

BOOST_FIXTURE_TEST_CASE(setAngle, StyleLabelTest)
{
    style->setAngle(25.5);
    BOOST_CHECK_EQUAL(25.5, style->angle());
}

BOOST_FIXTURE_TEST_CASE(setForegroundColor, StyleLabelTest)
{
    Color color(Color::Name::fire_brick);
    style->setForegroundColor(color);
    BOOST_CHECK(color == style->foregroundColor());
}

BOOST_FIXTURE_TEST_CASE(setBackgroundColor, StyleLabelTest)
{
    Color color(Color::Name::indigo);
    style->setBackgroundColor(color);
    BOOST_CHECK(color == style->backgroundColor());
}

BOOST_FIXTURE_TEST_CASE(setOutlineColor, StyleLabelTest)
{
    Color color(Color::Name::saddle_brown);
    style->setOutlineColor(color);
    BOOST_CHECK(color == style->outlineColor());
}

BOOST_FIXTURE_TEST_CASE(setShadowColor, StyleLabelTest)
{
    Color color(Color::Name::azure);
    style->setShadowColor(color);
    BOOST_CHECK(color == style->shadowColor());
}

BOOST_FIXTURE_TEST_CASE(setStretchFactor, StyleLabelTest)
{
    style->setStretchFactor(25.);
    BOOST_CHECK_EQUAL(25., style->stretchFactor());
}

BOOST_FIXTURE_TEST_CASE(setPlacement, StyleLabelTest)
{
    style->setPlacement(Label::Placement::last_vertex);
    BOOST_CHECK(Label::Placement::last_vertex == style->placement());
}

BOOST_FIXTURE_TEST_CASE(setAnchorPosition, StyleLabelTest)
{
    LabelAnchor anchor(LabelAnchor::Horizontal::left, LabelAnchor::Vertical::center);
    style->setAnchorPosition(anchor);
    BOOST_CHECK(anchor == style->anchorPosition());
}

BOOST_FIXTURE_TEST_CASE(setOffset, StyleLabelTest)
{
    style->setOffset(5., 2.);
    BOOST_CHECK_EQUAL(5., style->offset().x());
    BOOST_CHECK_EQUAL(2., style->offset().y());
}

BOOST_FIXTURE_TEST_CASE(setPerpendicularOffset, StyleLabelTest)
{
    style->setPerpendicularOffset(10);
    BOOST_CHECK_EQUAL(10, style->perpendicularOffset());
}

BOOST_AUTO_TEST_SUITE_END()
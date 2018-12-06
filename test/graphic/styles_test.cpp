#include <gtest/gtest.h>
#include "graphic/styles.h"

using namespace TL;
using namespace TL::graph;


class StylePenTest
  : public testing::Test
{
public:

protected:

  virtual void SetUp() override
  {
    pen_2.setPattern("-");
    pen_2.setPenCap(StylePen::PenCap::ROUND);
    pen_2.setPenColor(TL::Color(TL::Color::NAME::Aquamarine));
    pen_2.setPenJoin(StylePen::PenJoin::ROUNDED);
    pen_2.setPenName(StylePen::PenName::PEN_NULL);
    pen_2.setPenWidth(2);
    pen_2.setPerpendicularOffset(10);
    pen_2.setPriorityLevel(3);
  }

  virtual void TearDown() override
  {

  }

  StylePen pen_1;
  StylePen pen_2;
};


/* Constructor por defecto */

//TEST_F(StylePenTest, DefaultConstructor)
//{
  //EXPECT_STREQ("", pen_1.getPattern().c_str());
  //EXPECT_TRUE(pen_1.getPenCap() == StylePen::PenCap::BUTT);
  //EXPECT_TRUE(pen_1.getPenColor() == TL::Color());
  //EXPECT_TRUE(pen_1.getPenJoin() == StylePen::PenJoin::BEVEL);
  //EXPECT_TRUE(pen_1.getPenName() == StylePen::PenName::SOLID);
  //EXPECT_EQ(1, pen_1.getPenWidth());
  //EXPECT_EQ(0, pen_1.getPerpendicularOffset());
  //EXPECT_EQ(0, pen_1.getPriorityLevel());
//}

/* Constructor de copia */

//TEST_F(StylePenTest, CopyConstructor)
//{
//  StylePen style_pen(pen_2);
//  EXPECT_STREQ(pen_2.getPattern().c_str(), style_pen.getPattern().c_str());
//  EXPECT_TRUE(style_pen.getPenCap() == pen_2.getPenCap());
//  EXPECT_TRUE(style_pen.getPenColor() == pen_2.getPenColor());
//  EXPECT_TRUE(style_pen.getPenJoin() == pen_2.getPenJoin());
//  EXPECT_TRUE(style_pen.getPenName() == pen_2.getPenName());
//  EXPECT_EQ(pen_2.getPenWidth(), style_pen.getPenWidth());
//  EXPECT_EQ(pen_2.getPerpendicularOffset(), style_pen.getPerpendicularOffset());
//  EXPECT_EQ(pen_2.getPriorityLevel(), style_pen.getPriorityLevel());
//}
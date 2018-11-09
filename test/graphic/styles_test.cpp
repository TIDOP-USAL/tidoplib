#include <gtest/gtest.h>
#include <tidop/graphic/styles.h>


using namespace tl;
using namespace graph;

class StylePenTest
  : public testing::Test
{
public:

protected:

  virtual void SetUp() override
  {
    style = new graph::StylePen;
  }

  virtual void TearDown() override
  {
    if (style) {
      delete style;
      style = nullptr;
    }
  }

  graph::StylePen def_style;
  graph::StylePen *style;
};

TEST_F(StylePenTest, DefaultConstructor)
{
  graph::Color color;
  EXPECT_TRUE(color == def_style.color());
  EXPECT_EQ(1, def_style.width());
  EXPECT_STREQ("", def_style.pattern().c_str());
  EXPECT_EQ(StylePen::Name::solid, def_style.name());
  EXPECT_EQ(StylePen::Cap::butt, def_style.cap());
  EXPECT_EQ(StylePen::Join::bevel, def_style.join());
  EXPECT_EQ(0, def_style.perpendicularOffset());
  EXPECT_EQ(0, def_style.priorityLevel());
}

TEST_F(StylePenTest, setColor)
{
  graph::Color color(graph::Color::Name::beige);
  style->setColor(color);
  EXPECT_TRUE(color == style->color());
}

TEST_F(StylePenTest, setWidth)
{
  style->setWidth(5);
  EXPECT_EQ(5, style->width());
}

TEST_F(StylePenTest, setPattern)
{
  style->setPattern("4px 5px");
  EXPECT_STREQ("4px 5px", style->pattern().c_str());
}

TEST_F(StylePenTest, setName)
{
  style->setName(StylePen::Name::dash_dot_line);
  EXPECT_EQ(StylePen::Name::dash_dot_line, style->name());
}

TEST_F(StylePenTest, setCap)
{
  style->setCap(StylePen::Cap::projective);
  EXPECT_EQ(StylePen::Cap::projective, style->cap());
}

TEST_F(StylePenTest, setJoin)
{
  style->setJoin(StylePen::Join::rounded);
  EXPECT_EQ(StylePen::Join::rounded, style->join());
}

TEST_F(StylePenTest, setPerpendicularOffset)
{
  style->setPerpendicularOffset(25);
  EXPECT_EQ(25, style->perpendicularOffset());
}

TEST_F(StylePenTest, setPriorityLevel)
{
  style->setPriorityLevel(15);
  EXPECT_EQ(15, style->priorityLevel());
}





class StyleBrushTest
  : public testing::Test
{
public:

protected:

  virtual void SetUp() override
  {
  }

  virtual void TearDown() override
  {
  }

  StyleBrush def_style;
};


TEST_F(StyleBrushTest, DefaultConstructor)
{
  graph::Color color;
  EXPECT_TRUE(color == def_style.foreColor());
  EXPECT_TRUE(color == def_style.backColor());
  EXPECT_EQ(StyleBrush::Name::solid, def_style.brushName());
  EXPECT_EQ(0., def_style.angle());
  EXPECT_EQ(1., def_style.scalingFactor());
  EXPECT_EQ(0., def_style.spacingX());
  EXPECT_EQ(0., def_style.spacingY());
  EXPECT_EQ(0, def_style.priorityLevel());
}

TEST_F(StyleBrushTest, setForeColor)
{
  graph::Color color(graph::Color::Name::indigo);
  def_style.setForeColor(color);
  EXPECT_TRUE(color == def_style.foreColor());
}

TEST_F(StyleBrushTest, setBackColor)
{
  graph::Color color(graph::Color::Name::indigo);
  def_style.setBackColor(color);
  EXPECT_TRUE(color == def_style.backColor());
}

TEST_F(StyleBrushTest, setBrushName)
{
  def_style.setBrushName(StyleBrush::Name::null);
  EXPECT_EQ(StyleBrush::Name::null, def_style.brushName());
}

TEST_F(StyleBrushTest, setAngle)
{
  def_style.setAngle(.45);
  EXPECT_EQ(0.45, def_style.angle());
}

TEST_F(StyleBrushTest, setScalingFactor)
{
  def_style.setScalingFactor(2.);
  EXPECT_EQ(2., def_style.scalingFactor());
}

TEST_F(StyleBrushTest, setSpacing)
{
  def_style.setSpacing(2., 5.);
  EXPECT_EQ(2., def_style.spacingX());
  EXPECT_EQ(5., def_style.spacingY());
}








class StyleSymbolTest
  : public testing::Test
{
public:

protected:

  virtual void SetUp() override
  {
    style = new StyleSymbol;
  }

  virtual void TearDown() override
  {
    if (style) {
      delete style;
      style = nullptr;
    }
  }

  StyleSymbol def_style;
  StyleSymbol *style;
};


TEST_F(StyleSymbolTest, DefaultConstructor)
{
  EXPECT_EQ(0., def_style.angle());
  EXPECT_EQ(StyleSymbol::Name::cross, def_style.name());
  graph::Color color;
  EXPECT_TRUE(color == def_style.color());
  EXPECT_TRUE(color == def_style.outlineColor());
  EXPECT_EQ(1., def_style.scalingFactor());
  EXPECT_EQ(0., def_style.offsetX());
  EXPECT_EQ(0., def_style.offsetY());
  EXPECT_EQ(0, def_style.priorityLevel());
}

TEST_F(StyleSymbolTest, setAngle)
{
  style->setAngle(0.26);
  EXPECT_EQ(0.26, style->angle());
}

TEST_F(StyleSymbolTest, setName)
{
  style->setName(StyleSymbol::Name::circle);
  EXPECT_EQ(StyleSymbol::Name::circle, style->name());
}

TEST_F(StyleSymbolTest, setColor)
{
  graph::Color color(graph::Color::Name::gainsboro);
  style->setColor(color);
  EXPECT_TRUE(color == style->color());
}

TEST_F(StyleSymbolTest, setOutlineColor)
{
  graph::Color color(graph::Color::Name::dark_slate_gray);
  style->setOutlineColor(color);
  EXPECT_TRUE(color == style->outlineColor());
}

TEST_F(StyleSymbolTest, setScalingFactor)
{
  style->setScalingFactor(2.);
  EXPECT_EQ(2., style->scalingFactor());
}

TEST_F(StyleSymbolTest, setOffset)
{
  style->setOffset(5., 2.);
  EXPECT_EQ(5., style->offsetX());
  EXPECT_EQ(2., style->offsetY());
}



class StyleLabelTest
  : public testing::Test
{
public:

protected:

  virtual void SetUp() override
  {
    style = new StyleLabel;
  }

  virtual void TearDown() override
  {
    if (style) {
      delete style;
      style = nullptr;
    }
  }

  StyleLabel def_style;
  StyleLabel *style;
};


TEST_F(StyleLabelTest, DefaultConstructor)
{
  EXPECT_STREQ("", def_style.text().c_str());
  EXPECT_EQ(0., def_style.angle());
  graph::Color color;
  EXPECT_TRUE(color == def_style.foregroundColor());
  EXPECT_TRUE(color == def_style.backgroundColor());
  EXPECT_TRUE(color == def_style.outlineColor());
  EXPECT_TRUE(color == def_style.shadowColor());
  EXPECT_EQ(100., def_style.stretch());
  EXPECT_EQ(StyleLabel::LabelPlacement::p, def_style.labelPlacement());
  EXPECT_EQ(StyleLabel::AnchorPosition::vertical_baseline | StyleLabel::AnchorPosition::horizontal_left,
            def_style.anchorPosition());
  EXPECT_EQ(0., def_style.offsetX());
  EXPECT_EQ(0., def_style.offsetY());
  EXPECT_EQ(0., def_style.perpendicularOffset());
}

TEST_F(StyleLabelTest, setText)
{
  style->setText("label");
  EXPECT_STREQ("label", style->text().c_str());
}

TEST_F(StyleLabelTest, setAngle)
{
  style->setAngle(25.5);
  EXPECT_EQ(25.5, style->angle());
}

TEST_F(StyleLabelTest, setForegroundColor)
{
  graph::Color color(graph::Color::Name::fire_brick);
  style->setForegroundColor(color);
  EXPECT_TRUE(color == style->foregroundColor());
}

TEST_F(StyleLabelTest, setBackgroundColor)
{
  graph::Color color(graph::Color::Name::indigo);
  style->setBackgroundColor(color);
  EXPECT_TRUE(color == style->backgroundColor());
}

TEST_F(StyleLabelTest, setOutlineColor)
{
  graph::Color color(graph::Color::Name::saddle_brown);
  style->setOutlineColor(color);
  EXPECT_TRUE(color == style->outlineColor());
}

TEST_F(StyleLabelTest, setShadowColor)
{
  graph::Color color(graph::Color::Name::azure);
  style->setShadowColor(color);
  EXPECT_TRUE(color == style->shadowColor());
}

TEST_F(StyleLabelTest, setStretch)
{
  style->setStretch(25.);
  EXPECT_EQ(25., style->stretch());
}

TEST_F(StyleLabelTest, setLabelPlacement)
{
  style->setLabelPlacement(StyleLabel::LabelPlacement::l);
  EXPECT_EQ(StyleLabel::LabelPlacement::l, style->labelPlacement());
}

TEST_F(StyleLabelTest, setAnchorPosition)
{
  style->setAnchorPosition(StyleLabel::AnchorPosition::vertical_center | StyleLabel::AnchorPosition::horizontal_left);
  EXPECT_EQ(StyleLabel::AnchorPosition::vertical_center | StyleLabel::AnchorPosition::horizontal_left, style->anchorPosition());
}

TEST_F(StyleLabelTest, setOffset)
{
  style->setOffset(5., 2.);
  EXPECT_EQ(5., style->offsetX());
  EXPECT_EQ(2., style->offsetY());
}

TEST_F(StyleLabelTest, setPerpendicularOffset)
{
  style->setPerpendicularOffset(10.);
  EXPECT_EQ(10., style->perpendicularOffset());
}

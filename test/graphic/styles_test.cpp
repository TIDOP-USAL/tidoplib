#include <gtest/gtest.h>
#include <tidop/graphic/styles.h>


using namespace TL;
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
  EXPECT_EQ(TL::Color(0), def_style.color());
  EXPECT_EQ(1, def_style.width());
  EXPECT_EQ("", def_style.pattern().c_str());
  EXPECT_EQ(StylePen::Name::solid, def_style.name());
  EXPECT_EQ(StylePen::Cap::butt, def_style.cap());
  EXPECT_EQ(StylePen::Join::bevel, def_style.join());
  EXPECT_EQ(0, def_style.perpendicularOffset());
  EXPECT_EQ(0, def_style.priorityLevel());
}
#include <gtest/gtest.h>
#include <tidop/graphic/canvas.h>

using namespace TL;
using namespace graph;

class CanvasTest
  : public testing::Test
{
public:

protected:

  virtual void SetUp() override
  {
    canvas2 = new CanvasCV();
  }

  virtual void TearDown() override
  {
    if (canvas2) {
      delete canvas2;
      canvas2 = nullptr;
    }
  }

  CanvasCV canvas1;
  Canvas *canvas2;
};


TEST_F(CanvasTest, DefaultConstructor)
{
  graph::Color bg_color(graph::Color::NAME::White);

  EXPECT_EQ(100, canvas1.height());
  EXPECT_EQ(100, canvas1.width());
  EXPECT_EQ(bg_color, canvas1.backgroundColor());

  EXPECT_EQ(100, canvas2->height());
  EXPECT_EQ(100, canvas2->width());
  EXPECT_EQ(bg_color, canvas2->backgroundColor());
}

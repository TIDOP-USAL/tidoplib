#include <gtest/gtest.h>
#include <tidop/graphic/font.h>


using namespace tl;
using namespace graph;

class FontTest
  : public testing::Test
{
public:

protected:

  virtual void SetUp() override
  {
    font = new Font;
  }

  virtual void TearDown() override
  {
    if (font) {
      delete font;
      font = nullptr;
    }
  }

  Font defFont;
  Font *font;
};

TEST_F(FontTest, DefaultConstructor)
{
  EXPECT_STREQ("Arial", defFont.name().c_str());
  EXPECT_EQ(10, defFont.size());
  EXPECT_EQ(false, defFont.isBold());
  EXPECT_EQ(false, defFont.isItalic());
  EXPECT_EQ(false, defFont.isUnderline());
  EXPECT_EQ(false, defFont.isStrikethrough());
}

TEST_F(FontTest, setName)
{
  font->setName("Times New Roman");
  EXPECT_STREQ("Times New Roman", font->name().c_str());
}

TEST_F(FontTest, setSize)
{
  font->setSize(14);
  EXPECT_EQ(14, font->size());
}

TEST_F(FontTest, setBold)
{
  font->setBold(true);
  EXPECT_EQ(true, font->isBold());
}

TEST_F(FontTest, setItalic)
{
  font->setItalic(true);
  EXPECT_EQ(true, font->isItalic());
}

TEST_F(FontTest, setUnderline)
{
  font->setUnderline(true);
  EXPECT_EQ(true, font->isUnderline());
}

TEST_F(FontTest, setStrikethrough)
{
  font->setStrikethrough(true);
  EXPECT_EQ(true, font->isStrikethrough());
}

TEST_F(FontTest, setStyle)
{
  font->setStyle(Font::Style::bold);
  EXPECT_EQ(true, font->isBold());
  EXPECT_EQ(false, font->isItalic());

  font->setStyle(Font::Style::italic);
  EXPECT_EQ(false, font->isBold());
  EXPECT_EQ(true, font->isItalic());

  font->setStyle(Font::Style::normal);
  EXPECT_EQ(false, font->isBold());
  EXPECT_EQ(false, font->isItalic());

  font->setStyle(Font::Style::bold | Font::Style::italic);
  EXPECT_EQ(true, font->isBold());
  EXPECT_EQ(true, font->isItalic());
}
